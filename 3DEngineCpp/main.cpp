#include "3DEngine.h"
#include "freeLook.h"
#include "freeMove.h"

class TestGame : public Game {
public:
	TestGame() {}
	
	virtual void init();
protected:
private:
	TestGame(const TestGame& other) {}
	void operator=(const TestGame& other) {}
};

void TestGame::init() {
	GameObject* planeObject = new GameObject();
	GameObject* pointLightObject = new GameObject();
	GameObject* spotLightObject = new GameObject();
	GameObject* directionalLightObject = new GameObject();

	planeObject->addComponent(new ModelRenderer(new Model("./res/models/plane4.obj"), new Material(new Texture("bricks.jpg"), 0.5F, 4, new Texture("bricks_normal.jpg"), new Texture("bricks_disp.png"), 0.03F, -0.5F)));
	planeObject->getTransformer().setPos(Vector3f(0, -1, 5));
	planeObject->getTransformer().setScale(4.0f);
	
	pointLightObject->addComponent(new PointLight(Vector3f(0, 1, 0), 0.4f, Attenuation(0, 0, 1)));
	pointLightObject->getTransformer().setPos(Vector3f(7, 0, 7));
	
	spotLightObject->addComponent(new SpotLight(Vector3f(0, 1, 1), 0.4f, Attenuation(0, 0, 0.1f), 0.7f));
	spotLightObject->getTransformer().setRot(Quaternion(Vector3f(0, 1, 0), toRadians(90.0f)));
	
	directionalLightObject->addComponent(new DirectionalLight(Vector3f(1, 1, 1), 0.4f));
	
	GameObject* testMesh1 = new GameObject();
	GameObject* testMesh2 = new GameObject();
	
	testMesh1->addComponent(new ModelRenderer(new Model("./res/models/plane3.obj"), new Material(new Texture("bricks2.jpg"), 1, 8, new Texture("bricks2_normal.png"), new Texture("bricks2_disp.jpg"), 0.04F, -1.0F)));
	testMesh2->addComponent(new ModelRenderer(new Model("./res/models/plane3.obj"), new Material(new Texture("bricks2.jpg"), 1, 8, new Texture("bricks2_normal.png"), new Texture("bricks2_disp.jpg"), 0.04F, -1.0F)));
	
	testMesh1->getTransformer().setPos(Vector3f(0, 2, 0));
	testMesh1->getTransformer().setRot(Quaternion(Vector3f(0, 1, 0), 0.4f));
	testMesh1->getTransformer().setScale(1.0f);
	
	testMesh2->getTransformer().setPos(Vector3f(0, 0, 25));
	
	testMesh1->addChild(testMesh2);
	
	addToScene(planeObject);
	addToScene(pointLightObject);
	addToScene(spotLightObject);
	addToScene(directionalLightObject);
	addToScene(testMesh1);
	testMesh2->addChild((new GameObject())
		->addComponent(new Camera(Matrix4f().initPerspective(toRadians(70.0f), Window::getAspect(), 0.1f, 1000.0f)))
		->addComponent(new FreeLook())
		->addComponent(new FreeMove()));
	
	directionalLightObject->getTransformer().setRot(Quaternion(Vector3f(1, 0, 0), toRadians(-45)));
}

int main() {
	TestGame game;
	CoreEngine engine(800, 600, 60, &game);
	engine.createWindow("Ricirius");
	engine.start();
	
	return 0;
}
