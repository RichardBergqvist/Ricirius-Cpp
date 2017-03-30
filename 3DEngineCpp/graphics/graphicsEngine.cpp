#include "../graphics/graphicsEngine.h"
#include "../graphics/window.h"
#include "../graphics/model.h"
#include "../graphics/shader.h"
#include <GL/glew.h>
#include <cassert>

const Matrix4f GraphicsEngine::s_biasMatrix = Matrix4f().initScale(Vector3f(0.5, 0.5, 0.5)) * Matrix4f().initTranslation(Vector3f(1.0, 1.0, 1.0));

GraphicsEngine::GraphicsEngine() {
	m_samplerMap.insert(std::pair<std::string, unsigned int>("diffuse", 0));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("normalMap", 1));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("dispMap", 2));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("shadowMap", 3));
	
	setVector3f("ambient", Vector3f(0.2f, 0.2f, 0.2f));
	//setTexture("shadowMap", new Texture(1024, 1024, 0, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0));
	//setTexture("shadowMapTempTarget", new Texture(1024, 1024, 0, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0));
	m_defaultShader = new Shader("forward-ambient");
	m_shadowMapShader = new Shader("shadowMapGenerator");
	m_nullFilter = new Shader("filter-null");
	m_gausBlurFilter = new Shader("filter-gausBlur7x1");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);

	m_altCamera = new Camera(Matrix4f().initIdentity());
	m_altCameraObject = (new GameObject())->addComponent(m_altCamera);
	m_altCamera->getTransformer().rotate(Vector3f(0, 1, 0), toRadians(180.0F));

	int width = Window::getWidth();
	int height = Window::getHeight();

	m_tempTarget = new Texture(width, height, 0, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0);

	m_planeMaterial = new Material(m_tempTarget, 1, 8);
	m_planeTransformer.setScale(1);
	m_planeTransformer.rotate(Quaternion(Vector3f(1, 0, 0), toRadians(90.0F)));
	m_planeTransformer.rotate(Quaternion(Vector3f(0, 0, 1), toRadians(180.0F)));
	m_plane = new Model("./res/models/plane.obj");

	for (int i = 0; i < s_numShadowMaps; i++) {
		int shadowMapSize = 1 << (i + 1);
		m_shadowMaps[i] = new Texture(shadowMapSize, shadowMapSize, 0, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0);
		m_shadowMapTempTargets[i] = new Texture(shadowMapSize, shadowMapSize, 0, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0);
	}

	m_lightMatrix = Matrix4f().initScale(Vector3f(0, 0, 0));
}

GraphicsEngine::~GraphicsEngine() {
	setTexture("shadowMap", 0);

	if (m_defaultShader) delete m_defaultShader;
	if (m_shadowMapShader) delete m_shadowMapShader;
	if (m_nullFilter) delete m_nullFilter;
	if (m_gausBlurFilter) delete m_gausBlurFilter;
	if (m_altCameraObject) delete m_altCameraObject;
	if (m_planeMaterial) delete m_planeMaterial;
	if (m_plane) delete m_plane;

	for (int i = 0; i < s_numShadowMaps; i++) {
		if (m_shadowMaps[i]) delete m_shadowMaps[i];
		if (m_shadowMapTempTargets[i]) delete m_shadowMapTempTargets[i];
	}
}

void GraphicsEngine::blurShadowMap(int shadowMapIndex, float blurAmount) {
	Texture* shadowMap = m_shadowMaps[shadowMapIndex];
	Texture* tempTarget = m_shadowMapTempTargets[shadowMapIndex];
	setVector3f("blurScale", Vector3f(blurAmount / (shadowMap->getWidth()), 0.0F, 0.0F));
	applyFilter(m_gausBlurFilter, shadowMap, tempTarget);

	setVector3f("blurScale", Vector3f(0.0F, blurAmount / (shadowMap->getHeight()), 0.0F));
	applyFilter(m_gausBlurFilter, tempTarget, shadowMap);
}

void GraphicsEngine::applyFilter(Shader* filter, Texture* source, Texture* dest) {
	assert(source != dest);
	if (dest == 0)
		Window::bindAsRenderTarget();
	else
		dest->bindAsRenderTarget();

	setTexture("filterTexture", source);

	m_altCamera->setProjection(Matrix4f().initIdentity());
	m_altCamera->getTransformer().setPos(Vector3f(0, 0, 0));
	m_altCamera->getTransformer().setRot(Quaternion(Vector3f(0, 1, 0), toRadians(180.0F)));

	Camera* temp = m_mainCamera;
	m_mainCamera = m_altCamera;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	filter->bind();
	filter->updateUniforms(m_planeTransformer, *m_planeMaterial, this);
	m_plane->render();

	m_mainCamera = temp;
	setTexture("filterTexture", 0);
}

void GraphicsEngine::render(GameObject* object) {
	Window::bindAsRenderTarget();

	glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	object->renderAll(m_defaultShader, this);
	
	for(unsigned int i = 0; i < m_lights.size(); i++) {
		m_activeLight = m_lights[i];
		ShadowData* shadowData = m_activeLight->getShadowData();

		int shadowMapIndex = 0;
		if (shadowData)
			shadowMapIndex = shadowData->getShadowMapSizeAsPowerOf2() - 1;

		setTexture("shadowMap", m_shadowMaps[shadowMapIndex]);
		m_shadowMaps[shadowMapIndex]->bindAsRenderTarget();
		glClearColor(1.0F, 1.0F, 0.0F, 0.0F);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		if (shadowData) {
			m_altCamera->setProjection(shadowData->getProjection());
			ShadowCameraTransformer shadowCameraTransformer = m_activeLight->calcShadowCameraTransformer(m_mainCamera->getTransformer().getTransformedPos(), m_mainCamera->getTransformer().getTransformedRot());
			m_altCamera->getTransformer().setPos(shadowCameraTransformer.pos);
			m_altCamera->getTransformer().setRot(shadowCameraTransformer.rot);

			m_lightMatrix = s_biasMatrix * m_altCamera->getViewProjection();
			
			setFloat("shadowVarianceMin", shadowData->getVarianceMin());
			setFloat("shadowLightBleedingReduction", shadowData->getLightBleedReductionAmount());
			bool flipFaces = shadowData->getFlipFaces();

			Camera* temp = m_mainCamera;
			m_mainCamera = m_altCamera;

			if (flipFaces) glCullFace(GL_FRONT);
				object->renderAll(m_shadowMapShader, this);
			if (flipFaces) glCullFace(GL_BACK);

			m_mainCamera = temp;

			float shadowSoftness = shadowData->getShadowSoftness();
			if (shadowSoftness != 0)
				blurShadowMap(shadowMapIndex, shadowSoftness);
		} else {
			m_lightMatrix = Matrix4f().initScale(Vector3f(0, 0, 0));
			setFloat("shadowVarianceMin", 0.00002F);
			setFloat("shadowLightBleedingReduction", 0);
		}

		Window::bindAsRenderTarget();
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		object->renderAll(m_activeLight->getShader(), this);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}
}
