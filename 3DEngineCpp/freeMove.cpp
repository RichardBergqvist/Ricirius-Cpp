#include "freeMove.h"

FreeMove::FreeMove(float speed, int forwardKey, int backKey, int leftKey, int rightKey) :
	m_speed(speed),
	m_forwardKey(forwardKey),
	m_backKey(backKey),
	m_leftKey(leftKey),
	m_rightKey(rightKey) {}
	
void FreeMove::input(float delta) {
	float movAmt = m_speed * delta;

	if(Input::getKey(m_forwardKey))
		move(getTransform().getRot().getForward(), movAmt);
	if(Input::getKey(m_backKey))
		move(getTransform().getRot().getBack(), movAmt);
	if(Input::getKey(m_leftKey))
		move(getTransform().getRot().getLeft(), movAmt);
	if(Input::getKey(m_rightKey))
		move(getTransform().getRot().getRight(), movAmt);
}

void FreeMove::move(const Vector3f& direction, float amt) {
	getTransform().setPos(getTransform().getPos() + (direction * amt));
}
