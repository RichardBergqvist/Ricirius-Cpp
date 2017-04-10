#include "gameObject.h"
#include "gameComponent.h"
#include "../core/coreEngine.h"

GameObject::~GameObject() {
	for(unsigned int i = 0; i < m_components.size(); i++)
		if(m_components[i])
			delete m_components[i];
	
	for(unsigned int i = 0; i < m_children.size(); i++)
		if(m_children[i])
			delete m_children[i];
}

GameObject* GameObject::addChild(GameObject* child) {
	m_children.push_back(child); 
	child->getTransformer()->setParent(&m_transformer);
	child->setEngine(m_coreEngine);
	return this;
}

GameObject* GameObject::addComponent(GameComponent* component) {
	m_components.push_back(component);
	component->setParent(this);
	return this;
}

void GameObject::processInputAll(const Input& input, float delta) {
	processInput(input, delta);

	for(unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->processInputAll(input, delta);
}

void GameObject::updateAll(float delta) {
	update(delta);

	for(unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->updateAll(delta);
}

void GameObject::renderAll(const Shader& shader, const GraphicsEngine& graphicsEngine) const {
	render(shader, graphicsEngine);

	for(unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->renderAll(shader, graphicsEngine);
}

void GameObject::processInput(const Input& input, float delta) {
	m_transformer.update();

	for(unsigned int i = 0; i < m_components.size(); i++)
		m_components[i]->processInput(input, delta);
}

void GameObject::update(float delta) {
	for(unsigned int i = 0; i < m_components.size(); i++)
		m_components[i]->update(delta);
}

void GameObject::render(const Shader& shader, const GraphicsEngine& graphicsEngine) const {
	for(unsigned int i = 0; i < m_components.size(); i++)
		m_components[i]->render(shader, graphicsEngine);
}

void GameObject::setEngine(CoreEngine* engine) {
	if(m_coreEngine != engine)
	{
		m_coreEngine = engine;
		
		for(unsigned int i = 0; i < m_components.size(); i++)
			m_components[i]->addToEngine(engine);

		for(unsigned int i = 0; i < m_children.size(); i++)
			m_children[i]->setEngine(engine);
	}
}

std::vector<GameObject*> GameObject::getAllAttached() {
	std::vector<GameObject*> result;
	
	for(unsigned int i = 0; i < m_children.size(); i++) {
		std::vector<GameObject*> childObjects = m_children[i]->getAllAttached();
		result.insert(result.end(), childObjects.begin(), childObjects.end());
	}
	
	result.push_back(this);
	return result;
}
