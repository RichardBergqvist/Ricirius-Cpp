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
		move(getTransformer().getRot().getForward(), movAmt);
	if(Input::getKey(m_backKey))
		move(getTransformer().getRot().getBack(), movAmt);
	if(Input::getKey(m_leftKey))
		move(getTransformer().getRot().getLeft(), movAmt);
	if(Input::getKey(m_rightKey))
		move(getTransformer().getRot().getRight(), movAmt);
}

void FreeMove::move(const Vector3f& direction, float amt) {
	getTransformer().setPos(getTransformer().getPos() + (direction * amt));
}
