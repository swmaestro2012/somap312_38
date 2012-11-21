#pragma once
#include "Object.h"
#include <Box2D\Box2D.h>
#include "GameScene.h"
#include "SmartPad.h"

class Hero : public Object
{
private:
	b2FixtureDef fixtureDef_body;
	b2FixtureDef fixtureDef_stick;

	b2CircleShape circleShape;
	b2Body *body;

	b2PolygonShape PolyShape;
	b2Body *stick;
	SDL_Rect stickPos,stickFrame;

	b2RevoluteJointDef jointdef;
	b2RevoluteJoint *joint;

	int jumpPower;
	bool ghost;
	int type;
	bool jump;
	bool falling;
	bool goRight,goLeft;
	int doShoot, doJump;
	b2Filter tristana,pantheon;
	int PlayerType; // Á¾·ù
	string name_texture;
public:
	Hero(int _type);
	~Hero(void);

	void MoveLeft();
	void MoveRight();

	bool OnSpeed;
	void GhostMode(bool onoff);
	void Update();
	void Render();
	void Init();
	void EventListener(SDL_Event *event);
	void SpEventListener(SP_InputEvent *event);
	void SetType(int _type);
};
