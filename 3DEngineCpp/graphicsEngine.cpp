#include "graphicsEngine.h"
#include "window.h"
#include "model.h"
#include "shader.h"
#include <GL/glew.h>
#include "model.h"
#include <cstring>

GraphicsEngine::GraphicsEngine() {
	m_samplerMap.insert(std::pair<std::string, unsigned int>("diffuse", 0));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("normalMap", 1));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("dispMap", 2));
	
	setVector3f("ambient", Vector3f(0.2f, 0.2f, 0.2f));
	m_defaultShader = new Shader("forward-ambient");

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
}

GraphicsEngine::~GraphicsEngine() {
	if (m_defaultShader) delete m_defaultShader;
	if (m_altCameraObject) delete m_altCameraObject;
	if (m_planeMaterial) delete m_planeMaterial;
	if (m_plane) delete m_plane;
}

void GraphicsEngine::render(GameObject* object) {
	Window::bindAsRenderTarget();
	//m_tempTarget->bindAsRenderTarget();

	glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	object->renderAll(m_defaultShader, this);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);
	
	for(unsigned int i = 0; i < m_lights.size(); i++) {
		m_activeLight = m_lights[i];
		object->renderAll(m_activeLight->getShader(), this);
	}
	
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);

	//Window::bindAsRenderTarget();

	//Camera* temp = m_mainCamera;
	//m_mainCamera = m_altCamera;

	//glClearColor(0.0F, 0.0F, 0.5F, 1.0F);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//m_defaultShader->bind();
	//m_defaultShader->updateUniforms(m_planeTransformer, *m_planeMaterial, this);
	//m_plane->render();

	//m_mainCamera = temp;
}
