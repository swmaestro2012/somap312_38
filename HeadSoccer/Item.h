#pragma once
#include "Object.h"
#include "ResourceManager.h"

class Item : public Object
{
private:
	 int type;
	 int state;
	 
	 float StopWatch;
	 bool rightwind;
	 SDL_Rect back_pos;
	 SDL_Rect back_frame;
	 float backangle;
	 bool right;
public:
	Item();
	~Item(void);

	static enum item_type  {FAST,WIND,GHOST,BOUNCE};
	static enum State {SHOW,USE,DIE};

	void Update();
	void Render();
	void Using(float dt);

	int GetType() {return type;}
	void SetState(int _state) {state = _state;}
	int GetState() {return state;} 
	SDL_Point GetCenterPos();
	
	bool IsRightWind(){return rightwind;}
};
