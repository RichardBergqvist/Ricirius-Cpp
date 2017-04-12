#include "entity.h"
#include "entityComponent.h"
#include "../core/coreEngine.h"

Entity::~Entity() {
	for(unsigned int i = 0; i < m_components.size(); i++)
		if(m_components[i])
			delete m_components[i];
	
	for(unsigned int i = 0; i < m_children.size(); i++)
		if(m_children[i])
			delete m_children[i];
}

Entity* Entity::addChild(Entity* child) {
	m_children.push_back(child); 
	child->getTransformer()->setParent(&m_transformer);
	child->setEngine(m_coreEngine);
	return this;
}

Entity* Entity::addComponent(EntityComponent* component) {
	m_components.push_back(component);
	component->setParent(this);
	return this;
}

void Entity::processInputAll(const Input& input, float delta) {
	processInput(input, delta);

	for(unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->processInputAll(input, delta);
}

void Entity::updateAll(float delta) {
	update(delta);

	for(unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->updateAll(delta);
}

void Entity::renderAll(const Shader& shader, const GraphicsEngine& graphicsEngine, const Camera& camera) const {
	render(shader, graphicsEngine, camera);

	for(unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->renderAll(shader, graphicsEngine, camera);
}

void Entity::processInput(const Input& input, float delta) {
	m_transformer.update();

	for(unsigned int i = 0; i < m_components.size(); i++)
		m_components[i]->processInput(input, delta);
}

void Entity::update(float delta) {
	for(unsigned int i = 0; i < m_components.size(); i++)
		m_components[i]->update(delta);
}

void Entity::render(const Shader& shader, const GraphicsEngine& graphicsEngine, const Camera& camera) const {
	for(unsigned int i = 0; i < m_components.size(); i++)
		m_components[i]->render(shader, graphicsEngine, camera);
}

void Entity::setEngine(CoreEngine* engine) {
	if(m_coreEngine != engine)
	{
		m_coreEngine = engine;
		
		for(unsigned int i = 0; i < m_components.size(); i++)
			m_components[i]->addToEngine(engine);

		for(unsigned int i = 0; i < m_children.size(); i++)
			m_children[i]->setEngine(engine);
	}
}

std::vector<Entity*> Entity::getAllAttached() {
	std::vector<Entity*> result;
	
	for(unsigned int i = 0; i < m_children.size(); i++) {
		std::vector<Entity*> childObjects = m_children[i]->getAllAttached();
		result.insert(result.end(), childObjects.begin(), childObjects.end());
	}
	
	result.push_back(this);
	return result;
}
