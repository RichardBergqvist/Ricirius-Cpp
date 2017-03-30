#ifndef MODELRENDERER_H_INCLUDED
#define MODELRENDERER_H_INCLUDED

#include "../components/gameComponent.h"
#include "model.h"

class ModelRenderer : public GameComponent {
public:
	ModelRenderer(Model* model, Material* material) {
		m_model = model;
		m_material = material;
	}
	
	virtual ~ModelRenderer() {
		if(m_model) delete m_model;
		if(m_material) delete m_material;
	}

	virtual void render(Shader* shader, GraphicsEngine* graphicsEngine)  {
		shader->bind();
		shader->updateUniforms(getTransformer(), *m_material, graphicsEngine);
		m_model->render();
	}
protected:
private:
	Model* m_model;
	Material* m_material;
};

#endif // MODELRENDERER_H_INCLUDED
