#ifndef FREELOOK_H
#define FREELOOK_H

#include "../util/math3d.h"
#include "../components/gameComponent.h"
#include "../components/input.h"

class FreeLook : public GameComponent {
public:
	FreeLook(float sensitivity = 0.5f, int unlockMouseKey = Input::KEY_ESCAPE);
	
	virtual void input(float delta);
protected:
private:
	float m_sensitivity;
	int m_unlockMouseKey;
};

#endif // FREELOOK_H
