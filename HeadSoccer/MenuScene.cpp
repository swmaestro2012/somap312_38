#include "MenuScene.h"

MenuScene::MenuScene(void)
{
	ResourceManager::getInstance()->PushTexture("BlueParticle","Resource\\blueParticle.png");
	ResourceManager::getInstance()->PushTexture("RedParticle","Resource\\redParticle.png");

	ResourceManager::getInstance()->PushTexture("TEAMLOGO","Resource\\menu\\TLOGO.png");
	ResourceManager::getInstance()->PushTexture("MENUBG","Resource\\menu\\menubg.png");
	ResourceManager::getInstance()->PushTexture("START","Resource\\menu\\start.png");
	ResourceManager::getInstance()->PushTexture("OPTION","Resource\\menu\\option.png");
	ResourceManager::getInstance()->PushTexture("BUTTON_INFO","Resource\\menu\\button_info.png");
	ResourceManager::getInstance()->PushTexture("INFO","Resource\\menu\\info.png");
	ResourceManager::getInstance()->PushTexture("EXIT","Resource\\menu\\exit.png");
	ResourceManager::getInstance()->PushTexture("LOGO","Resource\\logo.png");
	
	select = START;
	OnSplash = true;
	logoalpha = 1;
	alphaplus = true;
	OnInfo = false;

	pos_info.x = 180;
	pos_info.y = 768/2;
	pos_info.w = 650;
	pos_info.h = 0;

	frame_info.x = frame_info.y = 0;
	frame_info.w = 650;
	frame_info.h = 400;
	
}

MenuScene::~MenuScene(void)
{
}

void MenuScene::Init()
{

}

#include "ParticleManager.h"
void MenuScene::Update( float dt )
{
	for(int i=0;i<rand()%9;i++){
			ParticleManager::add(1024/2,160,(rand()%100-50)*0.1,(rand()%30-15)*0.1,1);}
	ParticleManager::update();
}

void MenuScene::Render()
{
	if(OnSplash == true)
	{
		if(alphaplus == true)
		{
			logoalpha +=0.2;
			if(logoalpha >= 255)
				alphaplus = false;
		}
		else
		{
			logoalpha -= 0.2;
			if(logoalpha<0)
				OnSplash = false;
		}
		SDL_SetTextureAlphaMod(ResourceManager::getTexture("TEAMLOGO"),logoalpha);
		ResourceManager::Draw("TEAMLOGO",420,200,SDL_FLIP_NONE);
	}
	if(OnSplash == false)
	{
		SDL_SetTextureAlphaMod(ResourceManager::getTexture("START"),200);
		SDL_SetTextureAlphaMod(ResourceManager::getTexture("OPTION"),200);
		SDL_SetTextureAlphaMod(ResourceManager::getTexture("BUTTON_INFO"),200);
		SDL_SetTextureAlphaMod(ResourceManager::getTexture("EXIT"),200);

		switch(select)
		{
		case START:
			SDL_SetTextureAlphaMod(ResourceManager::getTexture("START"),255);
			break;
		case OPTION:
			SDL_SetTextureAlphaMod(ResourceManager::getTexture("OPTION"),255);
			break;
		case INFO:
			SDL_SetTextureAlphaMod(ResourceManager::getTexture("BUTTON_INFO"),255);
			break;
		case EXIT:
			SDL_SetTextureAlphaMod(ResourceManager::getTexture("EXIT"),255);
			break;
		}
		ResourceManager::Draw("MENUBG",0,0,SDL_FLIP_NONE);	
		ResourceManager::Draw("START",20,400,SDL_FLIP_NONE);
		ResourceManager::Draw("OPTION",270,400,SDL_FLIP_NONE);
		ResourceManager::Draw("BUTTON_INFO",520,400,SDL_FLIP_NONE);
		ResourceManager::Draw("EXIT",770,400,SDL_FLIP_NONE);

		ParticleManager::draw();
		ResourceManager::Draw("LOGO",110,90,SDL_FLIP_NONE);

		if(OnInfo == true)
		{
			if(pos_info.h <400)
			{
				pos_info.y -= 2;
				pos_info.h += 4;
			}
			ResourceManager::Draw("INFO",pos_info,frame_info);
		}
		else
		{
			if(pos_info.h>0)
			{
				pos_info.y += 2;
				pos_info.h -= 4;
			}
			ResourceManager::Draw("INFO",pos_info,frame_info);
		}
	}

}

void MenuScene::EventListener( SDL_Event *event )
{
	switch(event->type)
	{
	case SDL_KEYDOWN:
		switch(event->key.keysym.sym)
		{
		case SDLK_RIGHT:
			if(OnSplash == false)
			if(select < 3)
				select+=1;
			break;
		case SDLK_LEFT:
			if(OnSplash == false)
			if(select>0)
				select -= 1;
			break;
		case SDLK_RETURN:
			if(OnSplash == false)
			{
				switch(select)
				{
				case START:
					SceneManager::PushScene("GAMESCENE",new GameScene());
					SceneManager::ChangeScene("GAMESCENE");
					break;
				case OPTION:
					break;
				case INFO:
					if(!OnInfo)
						OnInfo = true;
					else
						OnInfo = false;
					break;
				case EXIT:
					exit(1);
					break;
				}
			}
			break;
		}
		break;
	}
}
