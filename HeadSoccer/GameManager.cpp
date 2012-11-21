#include "GameManager.h"

#include "particleManager.h"

SDL_Window *GameManager::window = NULL;
SDL_Renderer *GameManager::renderer = NULL;
GameManager::GameManager(void)
{
	window = SDL_CreateWindow("woong",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
		1024,768,SDL_WINDOW_SHOWN);
	//창모드 SDL_WINDOW_SHOWN
	renderer = SDL_CreateRenderer(window,-1,0);
	m_pSceneManager = new SceneManager();
	m_pResourceManager = ResourceManager::getInstance();
	quit = false;

	m_pSceneManager->PushScene("MENUSCENE",new MenuScene());
	m_pSceneManager->ChangeScene("MENUSCENE");
	nowTime = elapsedTime = 0;
	targetFrame = 60;
}

GameManager::~GameManager(void)
{
	delete m_pSceneManager;
	m_pResourceManager->getInstance()->destroyInstance();
}

void GameManager::MainLoop( )
{
	int st;
	st = 0;
	elapsedTime = 0;
	while(!quit)
	{
		nowTime = (elapsedTime) /1000 ; 
		st = SDL_GetTicks();
		while(SDL_PollEvent(&event))
		{
			m_pSceneManager->EventListener(&event);
			switch(event.type)
			{
			case SDL_QUIT:
				quit = true;
				break; 
			}
		}

//		if(SDL_GetTicks() % 30 == 0){
			ParticleManager::update();
//		}

		m_pSceneManager->Update(nowTime);
		
		SDL_RenderClear(renderer);
		m_pSceneManager->Render();
	
		SDL_RenderPresent(renderer);
		
		
		SDL_Delay(1);
		elapsedTime = SDL_GetTicks()-st;// 경과 시간
		
	/*	if(elapsedTime <  1000/targetFrame)
				SDL_Delay(1000/targetFrame-elapsedTime);*/
	}
}