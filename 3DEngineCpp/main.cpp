#include "3DEngine.h"
#include "../util/testing.h"
#include "../components/freeLook.h"
#include "../components/freeMove.h"
#include "../components/physicsEngineComponent.h"
#include "../components/physicsObjectComponent.h"

class TestGame : public Game {
public:
	TestGame() {}
	
	virtual void init(const Window& window);
protected:
private:
	TestGame(const TestGame& other) {}
	void operator=(const TestGame& other) {}
};

void TestGame::init(const Window& window) {
	Material bricks("bricks", Texture("bricks.jpg"), 0, 0, Texture("bricks_normal.jpg"), Texture("bricks_disp.png"), 0.03f, -0.5f);
	Material bricks2("bricks2", Texture("bricks2.jpg"), 0, 0, Texture("bricks2_normal.png"), Texture("bricks2_disp.jpg"), 0.04f, -1.0f);

	IndexedModel square;
	{
		square.AddVertex(1.0f, -1.0f, 0.0f);  square.addTexCoord(Vector2f(1.0f, 1.0f));
		square.AddVertex(1.0f, 1.0f, 0.0f);   square.addTexCoord(Vector2f(1.0f, 0.0f));
		square.AddVertex(-1.0f, -1.0f, 0.0f); square.addTexCoord(Vector2f(0.0f, 1.0f));
		square.AddVertex(-1.0f, 1.0f, 0.0f);  square.addTexCoord(Vector2f(0.0f, 0.0f));
		square.addFace(0, 1, 2); square.addFace(2, 1, 3);
	}
	Model customModel("square", square.finalize());

	//addToScene((new Entity(Vector3f(0, -1, 5), Quaternion(), 32.0f))->addComponent(new ModelRenderer(Model("terrain02.obj"), Material("bricks"))));
	//addToScene((new Entity(Vector3f(7, 0, 7)))->addComponent(new PointLight(Vector3f(0, 1, 0), 0.4f, Attenuation(0, 0, 1))));
	//addToScene((new Entity(Vector3f(20, -11, 5), Quaternion(Vector3f(1, 0, 0), toRadians(-60.0f)) * Quaternion(Vector3f(0, 1, 0), toRadians(90.0f))))->addComponent(new SpotLight(Vector3f(0, 1, 1), 0.4f, Attenuation(0, 0, 0.02f), toRadians(91.1f), 7, 1.0f, 0.5f)));
	addToScene((new Entity(Vector3f(), Quaternion(Vector3f(1, 0, 0), toRadians(-45))))->addComponent(new DirectionalLight(Vector3f(1, 1, 1), 0.4f, 10, 80.0f, 1.0f)));
	addToScene((new Entity(Vector3f(0, 2, 0), Quaternion(Vector3f(0, 1, 0), 0.4f), 1.0f))->addComponent(new ModelRenderer(Model("plane3.obj"), Material("bricks2")))->addChild((new Entity(Vector3f(0, 0, 25)))->addComponent(new ModelRenderer(Model("plane3.obj"), Material("bricks2")))->addChild((new Entity())->addComponent(new CameraComponent(Matrix4f().initPerspective(toRadians(70.0f), window.getAspect(), 0.1f, 1000.0f)))->addComponent(new FreeLook(window.getCenter()))->addComponent(new FreeMove(10)))));
	//addToScene((new Entity(Vector3f(24, -2, 5), Quaternion(Vector3f(0, 1, 0), toRadians(30.0f))))->addComponent(new ModelRenderer(Model("sphere.obj"), Material("bricks"))));
	//addToScene((new Entity(Vector3f(0, 0, 7), Quaternion(), 1.0f))->addComponent(new ModelRenderer(Model("square"), Material("bricks2"))));

	PhysicsEngine physicsEngine;
	
	physicsEngine.addObject(PhysicsObject(Vector3f(0, 0, 0), Vector3f(0, 0, 1), 1));
	physicsEngine.addObject(PhysicsObject(Vector3f(0, 0, 10), Vector3f(0, 0, -1), 2));
	
	PhysicsEngineComponent* physicsEngineComponent = new PhysicsEngineComponent(physicsEngine);
	
	for (unsigned int i = 0; i < physicsEngineComponent->getPhysicsEngine().getNumObjects(); i++) {
		addToScene((new Entity(Vector3f(0, 0, 0), Quaternion(), physicsEngineComponent->getPhysicsEngine().getObject(i).getRadius()))->addComponent(new PhysicsObjectComponent(&physicsEngineComponent->getPhysicsEngine().getObject(i)))->addComponent(new ModelRenderer(Model("sphere.obj"), Material("bricks"))));
	}
	
	addToScene((new Entity())->addComponent(physicsEngineComponent));
}

#include <iostream>

int main() {
	Testing::runAllTests();

	TestGame game;
	Window window(800, 600, "Ricirius");
	GraphicsEngine renderer(window);

	CoreEngine engine(60, &window, &renderer, &game);
	engine.start();
	
	return 0;
}
