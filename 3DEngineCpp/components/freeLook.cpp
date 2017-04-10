#include "freeLook.h"
#include "../graphics/window.h"

void FreeLook::processInput(const Input& input, float delta) {
	if(input.getKey(m_unlockMouseKey)) {
		input.setCursor(true);
		m_mouseLocked = false;
	}

	if(m_mouseLocked) {
		Vector2f deltaPos = input.getMousePosition() - m_windowCenter;
		
		bool rotY = deltaPos.getX() != 0;
		bool rotX = deltaPos.getY() != 0;
			
		if(rotY)
			getTransformer()->rotate(Vector3f(0,1,0), toRadians(deltaPos.getX() * m_sensitivity));
		if(rotX)
			getTransformer()->rotate(getTransformer()->getRot()->getRight(), toRadians(deltaPos.getY() * m_sensitivity));
			
		if(rotY || rotX)
			input.setMousePosition(m_windowCenter);
	}

	if (input.getMouseDown(Input::LEFT_MOUSE)) {
		input.setCursor(false);
		input.setMousePosition(m_windowCenter);
		m_mouseLocked = true;
	}
}
