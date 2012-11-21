#pragma once
#include <Box2D/Box2D.h>
#include "ResourceManager.h"
#include "GameScene.h"


class BackGround
{
private:
	b2BodyDef groundBodyDef;
	b2Body *grounBody;
	b2PolygonShape groundBox;
public:
	BackGround(void);
	~BackGround(void);

	void Update(int maptype);
	void Render();
	void Render2(int maptype);
};
