#pragma once
#include "Scene.h"
#include "GameScene.h"
#include "ResourceManager.h"

class MenuScene : public Scene
{
private:
	enum choose{START,OPTION,INFO,EXIT};
	int select;
	bool OnSplash;

	//·Î°í
	float logoalpha;
	bool alphaplus;
	bool OnInfo;
	SDL_Rect pos_info;
	SDL_Rect frame_info;
public:
	MenuScene(void);
	~MenuScene(void);

	void Init();
	void Update(float dt);
	void Render();
	void EventListener(SDL_Event *event);
};
