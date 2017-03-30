#include "freeLook.h"
#include "../graphics/window.h"

bool mouseLocked = false;

FreeLook::FreeLook(float sensitivity, int unlockMouseKey) {
	m_sensitivity = sensitivity;
	m_unlockMouseKey = unlockMouseKey;
}

void FreeLook::input(float delta) {
	if(Input::getKey(m_unlockMouseKey)) {
		Input::setCursor(true);
		mouseLocked = false;
	}

	if(mouseLocked) {
		Vector2f centerPosition = Vector2f((float)Window::getWidth()/2.0f, (float)Window::getHeight()/2.0f);
		Vector2f deltaPos = Input::getMousePosition() - centerPosition;
		
		bool rotY = deltaPos.getX() != 0;
		bool rotX = deltaPos.getY() != 0;
			
		if(rotY)
			getTransformer().rotate(Vector3f(0,1,0), toRadians(deltaPos.getX() * m_sensitivity));
		if(rotX)
			getTransformer().rotate(getTransformer().getRot().getRight(), toRadians(deltaPos.getY() * m_sensitivity));
			
		if(rotY || rotX)
			Input::setMousePosition(centerPosition);
	}

	if(Input::getMouseDown(Input::LEFT_MOUSE)) {
		Vector2f centerPosition = Vector2f((float)Window::getWidth()/2.0f, (float)Window::getHeight()/2.0f);
		Input::setCursor(false);
		Input::setMousePosition(centerPosition);
		mouseLocked = true;
	}
}
