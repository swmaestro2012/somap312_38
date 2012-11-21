#pragma once
#include <Box2D/Box2D.h>
#include "Object.h"
#include "ResourceManager.h"
#include "GameScene.h"

class Ball : public Object
{
private:
	b2CircleShape circleShape;
	b2FixtureDef fixtureDef;
	b2Body *ball;
	b2Filter some;
public:
	Ball(void);
	~Ball(void);

	void Update();
	void Render();

	void Init();
	SDL_Point GetCenterPos();
	float GetRadius() { return circleShape.m_radius; }
	static int state;

	void BounceMode(bool onoff);
};
