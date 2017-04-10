#ifndef FREEMOVE_H
#define FREEMOVE_H

#include "../util/math3d.h"
#include "../components/gameComponent.h"

class FreeMove : public GameComponent {
public:
	FreeMove(float speed = 10.0f, int forwardKey = Input::KEY_W, int backKey = Input::KEY_S, int leftKey = Input::KEY_A, int rightKey = Input::KEY_D) : m_speed(speed), m_forwardKey(forwardKey), m_backKey(backKey), m_leftKey(leftKey), m_rightKey(rightKey) {}
	
	virtual void processInput(const Input& input, float delta);
protected:
private:
	void move(const Vector3f& direction, float amt);

	float m_speed;
	int m_forwardKey;
	int m_backKey;
	int m_leftKey;
	int m_rightKey;
};

#endif // FREEMOVE_H
