#pragma once
#include "SDL.h"
#include "sdl_gdiplus.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include  <Box2D/Box2D.h>
class ResourceManager;
class GameManager
{
private:
	SceneManager *m_pSceneManager;
	ResourceManager *m_pResourceManager;
	SDL_Event event;
	bool quit;

	float targetFrame;
	float elapsedTime;
	float nowTime;
	//
	
public:
	GameManager(void);
	~GameManager(void);

	void MainLoop();
	static SDL_Window *window;
	static SDL_Renderer *renderer;
};
