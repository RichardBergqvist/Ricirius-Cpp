#ifndef MODELRENDERER_H_INCLUDED
#define MODELRENDERER_H_INCLUDED

#include "../components/gameComponent.h"
#include "model.h"

class ModelRenderer : public GameComponent {
public:
	ModelRenderer(const Model& model, const Material& material) : m_model(model), m_material(material) {}

	virtual void render(const Shader& shader, const GraphicsEngine& graphicsEngine, const Camera& camera) const {
		shader.bind();
		shader.updateUniforms(getTransformer(), m_material, graphicsEngine, camera);
		m_model.render();
	}
protected:
private:
	Model m_model;
	Material m_material;
};

#endif // MODELRENDERER_H_INCLUDED
