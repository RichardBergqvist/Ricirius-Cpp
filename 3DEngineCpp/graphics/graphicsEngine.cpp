#include "graphicsEngine.h"
#include "window.h"
#include "../components/gameObject.h"
#include "shader.h"
#include <GL/glew.h>
#include "model.h"
#include <cassert>

const Matrix4f GraphicsEngine::BIAS_MATRIX = Matrix4f().initScale(Vector3f(0.5, 0.5, 0.5)) * Matrix4f().initTranslation(Vector3f(1.0, 1.0, 1.0));

GraphicsEngine::GraphicsEngine(const Window& window) : m_plane(Model("plane.obj")), m_window(&window), m_tempTarget(window.getWidth(), window.getHeight(), 0, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0), m_planeMaterial("graphicsEngine_filterPlane", m_tempTarget, 1, 8), m_defaultShader("forward-ambient"), m_shadowMapShader("shadowMapGenerator"), m_nullFilter("filter-null"), m_gausBlurFilter("filter-gausBlur7x1"), m_fxaaFilter("filter-fxaa"), m_altCameraTransformer(Vector3f(0, 0, 0), Quaternion(Vector3f(0, 1, 0), toRadians(180.0F))), m_altCamera(Matrix4f().initIdentity(), &m_altCameraTransformer) {
	setSamplerSlot("diffuse", 0);
	setSamplerSlot("normalMap", 1);
	setSamplerSlot("dispMap", 2);
	setSamplerSlot("shadowMap", 3);

	setSamplerSlot("filterTexture", 0);

	setVector3f("ambient", Vector3f(0.2f, 0.2f, 0.2f));

	setFloat("fxaaSpanMax", 8.0F);
	setFloat("fxaaReduceMin", 1.0 / 128.0F);
	setFloat("fxaaReduceMul", 1.0 / 4.0F);

	setTexture("displayTexture", Texture(m_window->getWidth(), m_window->getHeight(), 0, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	//glEnable(GL_MULTISAMPLE);

	m_planeTransformer.setScale(1.0f);
	m_planeTransformer.rotate(Quaternion(Vector3f(1, 0, 0), toRadians(90.0f)));
	m_planeTransformer.rotate(Quaternion(Vector3f(0, 0, 1), toRadians(180.0f)));

	for (int i = 0; i < NUM_SHADOW_MAPS; i++) {
		int shadowMapSize = 1 << (i + 1);
		m_shadowMaps[i] = Texture(shadowMapSize, shadowMapSize, 0, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0);
		m_shadowMapTempTargets[i] = Texture(shadowMapSize, shadowMapSize, 0, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0);
	}

	m_lightMatrix = Matrix4f().initScale(Vector3f(0, 0, 0));
}

void GraphicsEngine::blurShadowMap(int shadowMapIndex, float blurAmount) {
	setVector3f("blurScale", Vector3f(blurAmount / (m_shadowMaps[shadowMapIndex].getWidth()), 0.0f, 0.0f));
	applyFilter(m_gausBlurFilter, m_shadowMaps[shadowMapIndex], &m_shadowMapTempTargets[shadowMapIndex]);

	setVector3f("blurScale", Vector3f(0.0f, blurAmount / (m_shadowMaps[shadowMapIndex].getHeight()), 0.0f));
	applyFilter(m_gausBlurFilter, m_shadowMapTempTargets[shadowMapIndex], &m_shadowMaps[shadowMapIndex]);
}

void GraphicsEngine::applyFilter(const Shader& filter, const Texture& source, const Texture* dest) {
	assert(&source != dest);
	if (dest == 0) {
		m_window->bindAsRenderTarget();
	} else {
		dest->bindAsRenderTarget();
	}

	setTexture("filterTexture", source);

	m_altCamera.setProjection(Matrix4f().initIdentity());
	m_altCamera.getTransformer()->setPos(Vector3f(0, 0, 0));
	m_altCamera.getTransformer()->setRot(Quaternion(Vector3f(0, 1, 0), toRadians(180.0f)));

	//const Camera* temp = m_mainCamera;
	//m_mainCamera = m_altCamera;

	glClear(GL_DEPTH_BUFFER_BIT);
	filter.bind();
	filter.updateUniforms(m_planeTransformer, m_planeMaterial, *this, m_altCamera);
	m_plane.render();

	//m_mainCamera = temp;
	setTexture("filterTexture", 0);
}

void GraphicsEngine::render(const GameObject& object) {
	
	getTexture("displayTexture").bindAsRenderTarget();
	//m_window->bindAsRenderTarget();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	object.renderAll(m_defaultShader, *this, *m_mainCamera);

	for (unsigned int i = 0; i < m_lights.size(); i++) {
		m_activeLight = m_lights[i];
		ShadowData shadowData = m_activeLight->getShadowData();

		int shadowMapIndex = 0;
		if (shadowData.getShadowMapSizeAsPowerOf2() != 0)
			shadowMapIndex = shadowData.getShadowMapSizeAsPowerOf2() - 1;

		assert(shadowMapIndex >= 0 && shadowMapIndex < NUM_SHADOW_MAPS);

		setTexture("shadowMap", m_shadowMaps[shadowMapIndex]);
		m_shadowMaps[shadowMapIndex].bindAsRenderTarget();
		glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		if (shadowData.getShadowMapSizeAsPowerOf2() != 0) {
			m_altCamera.setProjection(shadowData.getProjection());
			ShadowCameraTransformer shadowCameraTransformer = m_activeLight->calcShadowCameraTransformer(m_mainCamera->getTransformer().getTransformedPos(), m_mainCamera->getTransformer().getTransformedRot());
			m_altCamera.getTransformer()->setPos(shadowCameraTransformer.getPos());
			m_altCamera.getTransformer()->setRot(shadowCameraTransformer.getRot());

			m_lightMatrix = BIAS_MATRIX * m_altCamera.getViewProjection();

			setFloat("shadowVarianceMin", shadowData.getMinVariance());
			setFloat("shadowLightBleedingReduction", shadowData.getLightBleedReductionAmount());
			bool flipFaces = shadowData.getFlipFaces();

			//const Camera* temp = m_mainCamera;
			//m_mainCamera = m_altCamera;

			if (flipFaces)
				glCullFace(GL_FRONT);

			object.renderAll(m_shadowMapShader, *this, m_altCamera);

			if (flipFaces)
				glCullFace(GL_BACK);

			//m_mainCamera = temp;

			float shadowSoftness = shadowData.getShadowSoftness();
			if (shadowSoftness != 0) {
				blurShadowMap(shadowMapIndex, shadowSoftness);
			}
		} else {
			m_lightMatrix = Matrix4f().initScale(Vector3f(0, 0, 0));
			setFloat("shadowVarianceMin", 0.00002f);
			setFloat("shadowLightBleedingReduction", 0.0f);
		}

		getTexture("displayTexture").bindAsRenderTarget();
		//m_window->bindAsRenderTarget();

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		object.renderAll(m_activeLight->getShader(), *this, *m_mainCamera);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}

	setVector3f("inverseFilterTextureSize", Vector3f(1.0f / getTexture("displayTexture").getWidth(), 1.0f / getTexture("displayTexture").getHeight(), 0.0f));
	applyFilter(m_fxaaFilter, getTexture("displayTexture"), 0);
}