#pragma once
#include <Box2D\Box2D.h>
#include "WContactListener.h"
#include "Scene.h"
#include "Hero.h"
#include "Ball.h"
#include "BackGround.h"
#include "ItemManager.h"
#include "smartpad.h"
#define FIRSTPLAYER 1
#define SECONDPLAYER 2

#define MAP_FIELD 3
#define MAP_ROAD 4
#define MAP_LOCKER 5

//백인 흑인 황인
#define WHITE 6 
#define BLACK 7
#define YELLOW 8

class Hero;
class Ball;
class BackGround;
class ItemManager;
class GameScene : public Scene
{
private:
	Hero* hero[2];
	Ball* ball;
	BackGround* background;
	WContactListener *ContactListener;
	ItemManager *items;

private: //선택
	SDL_Rect pos_arrow_mode;
	SDL_Rect pos_arrow_charcter1,
		pos_arrow_charcter2;
	int type_character1,type_character2;
private:
	enum gamestatelist{SELECT_MODE,SELECT_CHARACTER,PLAYING};
	enum gamemodelist{CLASSIC,ITEMMODE,TIMEATTACK,GOLDEN};
	int gamestate;
	int gamemode;
	int map;
	static float windpower;
	int score_1p,score_2p;
	float gametimer;

	int resultAnimation;
	bool showResult;
	bool choose_left; //결과창 선택

private:
	void Update_Playing(float dt);
	void Render_Playing();
	void Update_Select(float dt);
	void Render_Select();
public:
	GameScene(void);
	~GameScene(void);

	void Init();
	void Update(float dt);
	void Render();
	void EventListener(SDL_Event *event);
	void SpEventListener( SP_InputEvent *event );

	static b2World *PhysicsWorld;
public:
	static float GetWindPower() {return windpower;}
};

