#include "3DEngine.h"
#include "../components/freeLook.h"
#include "../components/freeMove.h"

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

	addToScene((new GameObject(Vector3f(0, -1, 5), Quaternion(), 32.0f))->addComponent(new ModelRenderer(Model("terrain02.obj"), Material("bricks"))));
	addToScene((new GameObject(Vector3f(7, 0, 7)))->addComponent(new PointLight(Vector3f(0, 1, 0), 0.4f, Attenuation(0, 0, 1))));
	addToScene((new GameObject(Vector3f(20, -11, 5), Quaternion(Vector3f(1, 0, 0), toRadians(-60.0f)) * Quaternion(Vector3f(0, 1, 0), toRadians(90.0f))))->addComponent(new SpotLight(Vector3f(0, 1, 1), 0.4f, Attenuation(0, 0, 0.02f), toRadians(91.1f), 7, 1.0f, 0.5f)));
	addToScene((new GameObject(Vector3f(), Quaternion(Vector3f(1, 0, 0), toRadians(-45))))->addComponent(new DirectionalLight(Vector3f(1, 1, 1), 0.4f, 10, 80.0f, 1.0f)));
	addToScene((new GameObject(Vector3f(0, 2, 0), Quaternion(Vector3f(0, 1, 0), 0.4f), 1.0f))->addComponent(new ModelRenderer(Model("plane3.obj"), Material("bricks2")))->addChild((new GameObject(Vector3f(0, 0, 25)))->addComponent(new ModelRenderer(Model("plane3.obj"), Material("bricks2")))->addChild((new GameObject())->addComponent(new CameraComponent(Matrix4f().initPerspective(toRadians(70.0f), window.getAspect(), 0.1f, 1000.0f)))->addComponent(new FreeLook(window.getCenter()))->addComponent(new FreeMove()))));
	addToScene((new GameObject(Vector3f(24, -2, 5), Quaternion(Vector3f(0, 1, 0), toRadians(30.0f))))->addComponent(new ModelRenderer(Model("cube.obj"), Material("bricks2"))));
	addToScene((new GameObject(Vector3f(0, 0, 7), Quaternion(), 1.0f))->addComponent(new ModelRenderer(Model("square"), Material("bricks2"))));
}

int main() {
	TestGame game;
	Window window(800, 600, "Ricirius");
	GraphicsEngine renderer(window);

	CoreEngine engine(60, &window, &renderer, &game);
	engine.start();
	
	return 0;
}
