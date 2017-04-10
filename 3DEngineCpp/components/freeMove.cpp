#include "freeMove.h"
	
void FreeMove::processInput(const Input& input, float delta) {
	float movAmt = m_speed * delta;

	if(input.getKey(m_forwardKey))
		move(getTransformer()->getRot()->getForward(), movAmt);
	if(input.getKey(m_backKey))
		move(getTransformer()->getRot()->getBack(), movAmt);
	if(input.getKey(m_leftKey))
		move(getTransformer()->getRot()->getLeft(), movAmt);
	if(input.getKey(m_rightKey))
		move(getTransformer()->getRot()->getRight(), movAmt);
}

void FreeMove::move(const Vector3f& direction, float amt) {
	getTransformer()->setPos(*getTransformer()->getPos() + (direction * amt));
}
