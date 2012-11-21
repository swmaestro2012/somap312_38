#pragma once
#include <list>
#include <math.h>
#include "Item.h"
#include "GameScene.h"
#include "Ball.h"
using namespace std;
class Item;
class ItemManager
{
private:
	list<Item>  item_list;
	list<Item>::iterator  item_iter;
	bool fast;
	bool wind;
	bool rightwind;
	bool ghost;
	bool bounce;

	int fastnum;
	int ghostnum;
	void InitList();

	float StopWatch;
	int CreateTime; // 아템나올떄
public:
	ItemManager(void);
	~ItemManager(void);

	void Update(float dt,SDL_Point ballCenterPos,float radius);
	void Render();

	bool IsFast() {return fast;}
	bool IsWind() {return wind;}
	bool IsRightWind() {return rightwind;}
	bool IsGhost() {return ghost;}
	bool IsBounce() {return bounce;}
	int	GetFastNum() {return fastnum;}
	int GetGhostNum() {return ghostnum;}

	void Init();
};
