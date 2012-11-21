#include "GameScene.h"
#include "ResourceManager.h"
#include "ParticleManager.h"
#include <time.h>
b2World* GameScene::PhysicsWorld = NULL;
float GameScene::windpower = NULL;
GameScene::GameScene(void)
{
	PhysicsWorld = new b2World(b2Vec2(0,30));
	PhysicsWorld->SetAllowSleeping(false);
	PhysicsWorld->SetContinuousPhysics(true);
	ContactListener = new WContactListener();
	PhysicsWorld->SetContactListener(ContactListener);
	srand((unsigned int)time(NULL));
	map = rand()%3 + 3;

	//배경
	switch(map)
	{
	case MAP_FIELD:
		ResourceManager::getInstance()->PushTexture("BACKGROUND","Resource\\background1.png");
		ResourceManager::getInstance()->PushTexture("GROUND","Resource\\ground1.png");
		break;
	case MAP_ROAD:
		ResourceManager::getInstance()->PushTexture("BACKGROUND","Resource\\background2.png");
		ResourceManager::getInstance()->PushTexture("GROUND","Resource\\ground2.png");
		break;
	case MAP_LOCKER:
		ResourceManager::getInstance()->PushTexture("BACKGROUND","Resource\\background3.png");
		ResourceManager::getInstance()->PushTexture("GROUND","Resource\\ground3.png");
		break;
	default:
		exit(1);
	}

	//선택
	ResourceManager::getInstance()->PushTexture("CLASSIC","Resource\\select\\classic.png");
	ResourceManager::getInstance()->PushTexture("ITEM","Resource\\select\\item.png");
	ResourceManager::getInstance()->PushTexture("TIMEATTACk","Resource\\select\\timeattack.png");
	ResourceManager::getInstance()->PushTexture("GOLDEN","Resource\\select\\golden.png");
	ResourceManager::getInstance()->PushTexture("ARROW","Resource\\select\\arrow.png");
	ResourceManager::getInstance()->PushTexture("WHITE","Resource\\select\\white.png");
	ResourceManager::getInstance()->PushTexture("BLACK","Resource\\select\\black.png");
	ResourceManager::getInstance()->PushTexture("YELLOW","Resource\\select\\yellow.png");
	//게임
	ResourceManager::getInstance()->PushTexture("LOGO","Resource\\logo.png");
	ResourceManager::getInstance()->PushTexture("NUMBER","Resource\\number.png");
	ResourceManager::getInstance()->PushTexture("PLAYER_WHITE","Resource\\circle2.png");
	ResourceManager::getInstance()->PushTexture("PLAYER_BLACK","Resource\\circle.png");
	ResourceManager::getInstance()->PushTexture("PLAYER_YELLOW","Resource\\circle3.png");
	ResourceManager::getInstance()->PushTexture("STICK","Resource\\stick2.png");
	ResourceManager::getInstance()->PushTexture("BALL","Resource\\ball.png");
	ResourceManager::getInstance()->PushTexture("GOALPOST","Resource\\goalpost.png");

	// 아이템
	ResourceManager::getInstance()->PushTexture("FAST","Resource\\fast.png");
	ResourceManager::getInstance()->PushTexture("WIND","Resource\\wind.png");
	ResourceManager::getInstance()->PushTexture("GHOST","Resource\\ghost.png");
	ResourceManager::getInstance()->PushTexture("BOUNCE","Resource\\bounce.png");
	ResourceManager::getInstance()->PushTexture("ITEMBACK","Resource\\item_support.png");
	
	//결과창
	ResourceManager::getInstance()->PushTexture("ResultFrame","Resource\\resultFrame.png");
	ResourceManager::getInstance()->PushTexture("1PWIN","Resource\\1pwin.png");
	ResourceManager::getInstance()->PushTexture("2PWIN","Resource\\2pwin.png");
	ResourceManager::getInstance()->PushTexture("DRAW","Resource\\draw.png");
	ResourceManager::getInstance()->PushTexture("RESTART","Resource\\restart.png");
	ResourceManager::getInstance()->PushTexture("MENU","Resource\\menu.png");

	ResourceManager::getInstance()->PushTexture("Black","Resource\\black.png");
	SDL_SetTextureBlendMode(ResourceManager::getTexture("Black"),SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(ResourceManager::getTexture("Black"),96);

	SDL_SetTextureBlendMode(ResourceManager::getTexture("LOGO"),SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(ResourceManager::getTexture("LOGO"),150);

	


	SDL_SetTextureBlendMode(ResourceManager::getTexture("RedParticle"),SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(ResourceManager::getTexture("BlueParticle"),SDL_BLENDMODE_ADD);

	hero[0] = new Hero(FIRSTPLAYER);
	hero[1] = new Hero(SECONDPLAYER);
	
	ball = new Ball();
	background = new BackGround();
	items = new ItemManager();


	showResult = false;
	resultAnimation = 0;

	windpower = 0.f;
	score_1p = score_2p = 0;
	gametimer = 100.f; 
	choose_left = true;
	gamestate = SELECT_MODE;
	gamemode = CLASSIC;

	pos_arrow_mode.x = 120;
	pos_arrow_mode.y = 350;
	pos_arrow_mode.w = pos_arrow_mode.h = 50;

	pos_arrow_charcter1.w = pos_arrow_charcter1.h = 
	pos_arrow_charcter2.w = pos_arrow_charcter2.h = 50;
	pos_arrow_charcter1.x = 70;	pos_arrow_charcter1.y = 700;
	pos_arrow_charcter2.x = 170;	pos_arrow_charcter2.y = 700;
	type_character1 = type_character2 = WHITE;
}

GameScene::~GameScene(void)
{
	delete hero[0];
	delete hero[1];
	delete ball;
	delete background;
	delete items;
}

void GameScene::Init()
{

}

void GameScene::Update(float dt)
{	
	if(gamestate == SELECT_MODE)
		Update_Select(dt);
	else if(gamestate ==PLAYING )
		Update_Playing(dt);
}

void GameScene::Render()
{
	if(gamestate == SELECT_MODE || gamestate == SELECT_CHARACTER)
		Render_Select();
	else if(gamestate == PLAYING)
		Render_Playing();
}



void GameScene::SpEventListener( SP_InputEvent *event )
{
	hero[0]->SpEventListener(event);
	hero[1]->SpEventListener(event);
}

void GameScene::Update_Playing(float dt)
{
	background->Update(map);
	if(!showResult){
		gametimer -= dt;
		if(gametimer<0)
			gametimer = 0; 
		if(gamemode == ITEMMODE) // 아이템모드일때만 아템생성
		items->Update(dt,ball->GetCenterPos(),ball->GetRadius());

		//가속도
		if(items->IsFast() == true)
		{
			if(items->GetFastNum() == FIRSTPLAYER)
				hero[0]->OnSpeed = true;//
			else if(items->GetFastNum() == SECONDPLAYER)
				hero[1]->OnSpeed = true;//
		}
		else
		{
			for(int i=0;i<=1;i++)
				hero[i]->OnSpeed = false;
		}
		//바람
		if(items->IsWind() == true)
		{
			if(items->IsRightWind() == true)
				windpower = 0.4f;
			else
				windpower = -0.4f;
		}
		else
		{
			windpower = 0.f;
		}
		//고스트
		if(items->IsGhost() == true)
		{
			if(items->GetGhostNum() == FIRSTPLAYER)
				hero[0]->GhostMode(true);
			else
				hero[1]->GhostMode(true);
		}
		else
		{
			hero[0]->GhostMode(false);
			hero[1]->GhostMode(false);
		}
		//바운스
		if(items->IsBounce() == true)
		{
			ball->BounceMode(true);
		}
		else
		{
			ball->BounceMode(false);
		}
		//아이템
		for(int i=0;i<=1;i++)
			hero[i]->Update();

		ball->Update();
		if(ball->GetPos().x <100.0f && ball->GetPos().y >538.0f)
		{
			ball->Init();
			items->Init();
			score_2p += 1;
			for(int i=0;i<=1;i++)
				hero[i]->Init();
		}
		else if(ball->GetPos().x > 924.f && ball->GetPos().y > 538.f)
		{
			ball->Init();
			items->Init();
			score_1p +=1;
			for(int i=0;i<=1;i++)
				hero[i]->Init();
		}

		if(GetAsyncKeyState(VK_ESCAPE))
			exit(1);
		PhysicsWorld->Step(1.0f/60.0f, 50, 100);
		PhysicsWorld->Step(1.0f/60.0f, 50, 100);
		PhysicsWorld->Step(1.0f/60.0f, 50, 100);
		PhysicsWorld->Step(1.0f/60.0f, 50, 100);
		
		//결과
		switch(gamemode)
		{
		case CLASSIC:
			if(score_1p == 7 || score_2p == 7)
				showResult = true;
			break;
		case ITEMMODE:
			if(score_1p == 7 || score_2p == 7)
				showResult = true;
			break;
		case TIMEATTACK:
			if(gametimer <=0 )
				showResult = true;
			break;
		case GOLDEN:
			if(score_1p == 1 || score_2p == 1)
				showResult = true;
			break;
		}
	}
}

void GameScene::Render_Playing()
{
	background->Render();
	ParticleManager::draw();
	background->Render2(map);

	for(int i=0;i<=1;i++)
		hero[i]->Render();
	ball->Render();
	items->Render();
	ResourceManager::getInstance()->Draw("GOALPOST",0,538,SDL_FLIP_NONE);
	ResourceManager::getInstance()->Draw("GOALPOST",924,538,SDL_FLIP_HORIZONTAL);

	SDL_Rect pos_number = {200,20,50,50};
	SDL_Rect frame_number = {50*(score_1p%10),0,50,50};
	ResourceManager::getInstance()->Draw("NUMBER",pos_number,frame_number);
	pos_number.x = 150;
	frame_number.x = 50*(score_1p/10);
	if(frame_number.x != 0)
		ResourceManager::getInstance()->Draw("NUMBER",pos_number,frame_number);

	pos_number.x = 800;
	frame_number.x = 50*(score_2p%10);
	ResourceManager::getInstance()->Draw("NUMBER",pos_number,frame_number);
	pos_number.x = 750;
	frame_number.x = 100*(score_2p/10);
	if(frame_number.x !=0)
		ResourceManager::getInstance()->Draw("NUMBER",pos_number,frame_number);

	if(gamemode == TIMEATTACK)
	{
		//분
		pos_number.x = 420;
		frame_number.x = 50*((int)gametimer/60);
		ResourceManager::getInstance()->Draw("NUMBER",pos_number,frame_number);
		//:
		pos_number.x = 470;
		frame_number.x = 500;
		ResourceManager::getInstance()->Draw("NUMBER",pos_number,frame_number);
		//10 초
		pos_number.x = 520;
		frame_number.x = 50*(((int)gametimer%60)/10);
		ResourceManager::getInstance()->Draw("NUMBER",pos_number,frame_number);
		//1초
		pos_number.x = 570;
		frame_number.x = 50*((int)gametimer%10);
		ResourceManager::getInstance()->Draw("NUMBER",pos_number,frame_number);
	}
	if(showResult){
		if(resultAnimation <= 350)
			resultAnimation +=2;

		SDL_Rect rt = {0,0,1024,768};
		SDL_Rect crt = {0,0,1,1};
		ResourceManager::Draw("Black",rt,crt,0,SDL_FLIP_NONE);

		rt.x = 1024/2-320;
		rt.y = 768/2 - resultAnimation/2;
		rt.w = 640;
		rt.h = resultAnimation;
		crt.x = crt.y = 0;
		crt.w = 640, crt.h = 350;
		ResourceManager::Draw("ResultFrame",rt,crt,0,SDL_FLIP_NONE);

		SDL_Rect tri;
		rt.x += 170;
		rt.w = 300;
		if(resultAnimation < 180)
		{
			rt.y = 768/2 - resultAnimation/4;
			rt.h = resultAnimation/2;
		}
		else
		{
			rt.y = 768/2 - 45;
			rt.h = 90;
		}
		crt.w =	375;
		crt.h = 90;

		if(score_1p > score_2p)
			ResourceManager::Draw("1PWIN",rt,crt,0,SDL_FLIP_NONE);
		else if(score_2p > score_1p)
			ResourceManager::Draw("2PWIN",rt,crt,0,SDL_FLIP_NONE);
		else if(score_1p == score_2p)
			ResourceManager::Draw("DRAW",rt,crt,0,SDL_FLIP_NONE);

		rt.x = 1024/2-250;
		rt.y = 768/2+80;
		rt.w = 200; 
		rt.h = 65;
		crt.w =	200;
		crt.h = 65;
		if(resultAnimation >= 350)
		{
			if(choose_left == true)
				rt.y -= 20;
			ResourceManager::Draw("RESTART",rt,crt,0,SDL_FLIP_NONE);
			if(choose_left == true)
				rt.y += 20; //복구 헤헤

			rt.x += 300;
			if(choose_left == false)
				rt.y -= 20;
			ResourceManager::Draw("MENU",rt,crt,0,SDL_FLIP_NONE);
			if(choose_left == false)
				rt.y += 20;
		}

	}


}

void GameScene::Update_Select( float dt )
{

}


void GameScene::Render_Select()
{
	ResourceManager::Draw("CLASSIC",50,100,SDL_FLIP_NONE);
	ResourceManager::Draw("ITEM",300,100,SDL_FLIP_NONE);
	ResourceManager::Draw("TIMEATTACk",550,100,SDL_FLIP_NONE);
	ResourceManager::Draw("GOLDEN",800,100,SDL_FLIP_NONE);
	ResourceManager::Draw("ARROW",pos_arrow_mode.x,pos_arrow_mode.y,SDL_FLIP_NONE);

	//if(gamestate == SELECT_CHARACTER)
	//{
		ResourceManager::Draw("WHITE",100,450,SDL_FLIP_NONE);
		ResourceManager::Draw("BLACK",400,450,SDL_FLIP_NONE);
		ResourceManager::Draw("YELLOW",700,450,SDL_FLIP_NONE);
		ResourceManager::Draw("ARROW",pos_arrow_charcter1.x,pos_arrow_charcter1.y,SDL_FLIP_NONE);
		ResourceManager::Draw("ARROW",pos_arrow_charcter2.x,pos_arrow_charcter2.y,SDL_FLIP_NONE);
	//}
}

void GameScene::EventListener( SDL_Event *event )
{
	hero[0]->EventListener(event);
	hero[1]->EventListener(event);

	if(event->type == SDL_KEYDOWN)
	{
		//모드 선택
		if(gamestate == SELECT_MODE)
		{
			if(event->key.keysym.sym == SDLK_RIGHT)
			{
				if(gamemode < 3)
				{
					pos_arrow_mode.x += 250;
					gamemode +=1;
				}
			}
			else if(event->key.keysym.sym == SDLK_LEFT)
			{
				if(gamemode >0)
				{
					pos_arrow_mode.x -= 250;
					gamemode -= 1;
				}
			}
			else if(event->key.keysym.sym == SDLK_RETURN)
			{
				gamestate = SELECT_CHARACTER;
			}
		}

		//케릭터 선택
		else	if(gamestate == SELECT_CHARACTER)
		{
			//1p
			if(event->key.keysym.sym == SDLK_RIGHT)
			{
				if(type_character1 < YELLOW)
				{
					pos_arrow_charcter1.x += 350;
					type_character1 +=1; 
				}
			}
			else if(event->key.keysym.sym == SDLK_LEFT)
			{
				if(type_character1 > WHITE)
				{
					pos_arrow_charcter1.x -= 350;
					type_character1 -= 1;
				}
			}
			//2p
			else if(event->key.keysym.sym == SDLK_d)
			{
				if(type_character2 < YELLOW)
				{
					pos_arrow_charcter2.x += 350;
					type_character2 +=1; 
				}
			}
			else if(event->key.keysym.sym == SDLK_a)
			{
				if(type_character2> WHITE)
				{
					pos_arrow_charcter2.x -= 350;
					type_character2 -= 1;
				}
			}
			else if(event->key.keysym.sym == SDLK_RETURN)
			{
				gamestate = PLAYING;
				hero[0]->SetType(type_character1);
				hero[1]->SetType(type_character2);
			}
		}

		//겜중
		if(gamestate == PLAYING)
		{
			if(event->key.keysym.sym == SDLK_q)
			{
				showResult = true;
				resultAnimation = 1;
			}
			if(showResult)
			{
				if(event->key.keysym.sym == SDLK_LEFT)
				{
					choose_left = true;

				}
				if(event->key.keysym.sym == SDLK_RIGHT)
				{
					choose_left = false;
				}
				if(event->key.keysym.sym == SDLK_RETURN)
				{
					if(choose_left == true)
					{
						SceneManager::PopScene("GAMESCENE");
						SceneManager::PushScene("GAMESCENE",new GameScene());
						SceneManager::ChangeScene("GAMESCENE");
					}
					else
					{

						SceneManager::PopScene("GAMESCENE");
						SceneManager::ChangeScene("MENUSCENE");
					}
				}
			}
		}
	}

}