#include "Hero.h"
#include "ResourceManager.h"
#include "ParticleManager.h"

Hero::Hero(int _type)
{
	type = _type; //1p 2p
	jump = false;
	falling = false;
	Frame.w = Frame.h = Position.w = Position.h = 70;
	Frame.x = Frame.y = 0;
	OnSpeed = false;
	ghost = false;
	goRight = goLeft = false;
	doShoot = doJump = false;
	jumpPower  = 0;
	//////////////////////////////////////////////////////////////////////////
	//몸뚱이
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	if(type == FIRSTPLAYER)
		bodyDef.position.Set(150.f,650.f);
	else if(type == SECONDPLAYER)
		bodyDef.position.Set(850.f,650.f);

	if(type == FIRSTPLAYER)
		fixtureDef_body.userData = "stick_first";
	else if(type == SECONDPLAYER)
		fixtureDef_body.userData = "stick_second";
	circleShape.m_radius = 35.0f;
	fixtureDef_body.shape = &circleShape;
	fixtureDef_body.density = 300.f;
	fixtureDef_body.restitution = 0;
	fixtureDef_body.friction = 1.2; 
	fixtureDef_body.filter.groupIndex = 2;

	body = GameScene::PhysicsWorld->CreateBody(&bodyDef);
	body->SetLinearDamping(0.3);
	body->SetAngularDamping(2);
	body->CreateFixture(&fixtureDef_body);

	///////다리
	//원중심에다 막대두기
	if(_type == FIRSTPLAYER)
		bodyDef.position.Set(body->GetPosition().x+35,body->GetPosition().y-10);
	else
		bodyDef.position.Set(body->GetPosition().x+35,body->GetPosition().y+10);
	PolyShape.SetAsBox(10.0f,15.0f);

	stick = GameScene::PhysicsWorld->CreateBody(&bodyDef);
	fixtureDef_stick.shape = &PolyShape;
	fixtureDef_stick.friction = 10;
	fixtureDef_stick.density = 300;
	fixtureDef_stick.restitution = 0.4;

	printf("%d\t%d\n",fixtureDef_stick.filter.categoryBits,fixtureDef_stick.filter.maskBits);
	if(type == FIRSTPLAYER)
		fixtureDef_stick.userData = "stick_first";
	else if(type == SECONDPLAYER)
		fixtureDef_stick.userData = "stick_second";

	stick->CreateFixture(&fixtureDef_stick);
	stick->SetLinearDamping(0.5);
	stick->SetAngularDamping(10);

	stickPos.x = stick->GetPosition().x-40.0f;	stickPos.y = stick->GetPosition().y - 5.f;
	stickPos.w = 20;	stickPos.h = 30;
	stickFrame.x = stickFrame.y = 0;
	stickFrame.w = 20;	stickFrame.h = 30;

	//조인트
	jointdef.bodyA = body;
	jointdef.bodyB = stick;
	jointdef.Initialize(body,stick,body->GetPosition());

	joint = (b2RevoluteJoint*)GameScene::PhysicsWorld->CreateJoint(&jointdef);
}

Hero::~Hero(void)
{

}

void Hero::Update()
{
	body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x +GameScene::GetWindPower(),body->GetLinearVelocity().y));

	int x,y;
	x = body->GetPosition().x;
	y = body->GetPosition().y;

	if(type == FIRSTPLAYER)
	{
		if(rand() % 6 == 0){
			for(int i=0;i<rand()%3;i++){
				ParticleManager::add(x,y-25 + rand() % 40, -0.8f,0);
			}
		}
		if(OnSpeed == false)
		{
			if(GetAsyncKeyState(VK_RIGHT) || goRight)
				MoveRight();
			if(GetAsyncKeyState(VK_LEFT) || goLeft)
				MoveLeft();
		}
		else
		{
			if(GetAsyncKeyState(VK_RIGHT) || goRight)
				MoveRight();
			if(GetAsyncKeyState(VK_LEFT) || goLeft)
				MoveLeft();
		}


		if(GetAsyncKeyState(VK_DOWN))
			body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x,30.f));
		if((GetAsyncKeyState(VK_SPACE) || doShoot) && stick->GetAngle() >0  )
			stick->SetAngularVelocity(-10.f);


		if(stick->GetAngle() <=0){
			stick->SetTransform(b2Vec2(stick->GetPosition()),0);

		}

	}
	else if(type == SECONDPLAYER)
	{
		if(rand() % 6 == 0){
			for(int i=0;i<rand()%3;i++){
				ParticleManager::add(x,y-25 + rand() % 40, 0.8f,0);
			}
		}

		if(OnSpeed == false)
		{
			if(GetAsyncKeyState('D') || goRight)
				MoveRight();
			if(GetAsyncKeyState('A') || goLeft)
				MoveLeft();
		}
		else
		{
			if(GetAsyncKeyState('D') || goRight)
				MoveRight();
			if(GetAsyncKeyState('A') || goLeft)
				MoveLeft();
		}

		if(GetAsyncKeyState('S'))
			body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x,30.f));
		if(GetAsyncKeyState('Z') && stick->GetAngle() < 3.1 )
			stick->SetAngularVelocity(10.f);

		if(stick->GetAngle() >3.1)
			stick->SetTransform(stick->GetPosition(),3.1);
	}
	if(jump){
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x,-40.f));

		//jumpPower -=2;
	}
	if(body->GetPosition().y < 600.f){
		jump = false;
		falling = true;
	}
	if(body->GetPosition().y > 680.f)
		falling = false;

	if(doShoot != 0)
		doShoot --;
	if(doJump != 0)
		doJump --;

	SetPosition(body->GetPosition().x-35.0f,body->GetPosition().y-35.0f);
	angle = body->GetAngle()* 180 / 3.14156;
	stickPos.x = stick->GetPosition().x - 10.0f;
	stickPos.y = stick->GetPosition().y - 10.f;

}
void Hero::Render()
{
	if(OnSpeed){
		SDL_SetTextureColorMod(ResourceManager::getTexture(name_texture),255,0,0);
	}
	else{
		SDL_SetTextureColorMod(ResourceManager::getTexture(name_texture),255,255,255);
	}
	if(ghost){
		SDL_SetTextureAlphaMod(ResourceManager::getTexture(name_texture),128);
		SDL_SetTextureAlphaMod(ResourceManager::getTexture("STICK"),128);

		SDL_SetTextureBlendMode(ResourceManager::getTexture(name_texture),SDL_BLENDMODE_ADD);
		SDL_SetTextureBlendMode(ResourceManager::getTexture("STICK"),SDL_BLENDMODE_MOD);
	}
	else{
		SDL_SetTextureAlphaMod(ResourceManager::getTexture(name_texture),255);
		SDL_SetTextureAlphaMod(ResourceManager::getTexture(name_texture),255);

		SDL_SetTextureBlendMode(ResourceManager::getTexture(name_texture),SDL_BLENDMODE_BLEND);
		SDL_SetTextureBlendMode(ResourceManager::getTexture("STICK"),SDL_BLENDMODE_BLEND);
	}
	if(type == FIRSTPLAYER)
	{
		if(PlayerType == WHITE)
		ResourceManager::getInstance()->Draw("PLAYER_WHITE",Position,Frame,0,SDL_FLIP_NONE);	
		else if(PlayerType == BLACK)
			ResourceManager::getInstance()->Draw("PLAYER_BLACK",Position,Frame,0,SDL_FLIP_NONE);	
		else if(PlayerType == YELLOW)
			ResourceManager::getInstance()->Draw("PLAYER_YELLOW",Position,Frame,0,SDL_FLIP_NONE);	
		ResourceManager::getInstance()->Draw("STICK",stickPos,stickFrame,stick->GetAngle()* 180 / 3.14156,SDL_FLIP_NONE);	
	}
	else
	{
		if(PlayerType == WHITE)
			ResourceManager::getInstance()->Draw("PLAYER_WHITE",Position,Frame,0,SDL_FLIP_HORIZONTAL);	
		else if(PlayerType == BLACK)
			ResourceManager::getInstance()->Draw("PLAYER_BLACK",Position,Frame,0,SDL_FLIP_HORIZONTAL);	
		else if(PlayerType == YELLOW)
			ResourceManager::getInstance()->Draw("PLAYER_YELLOW",Position,Frame,0,SDL_FLIP_HORIZONTAL);	
		ResourceManager::getInstance()->Draw("STICK",stickPos,stickFrame,stick->GetAngle()* 180 / 3.14156,SDL_FLIP_VERTICAL);	
	}

}

void Hero::SpEventListener( SP_InputEvent *event )
{
	switch(event->type){
	case SP_INPUT_GYRO:
		printf("%f \n", event->gyro.x);
		if(event->gyro.x >= 0.3){
			goRight = true;
			goLeft = false;
		}
		else if(event->gyro.x <= -0.3){
			goLeft = true;
			goRight = false;
		}
		else{
			goRight = goLeft = false;
		}
		break;
	case SP_INPUT_TOUCH:
		{
			// shoot
			if(event->touch.x >= 240){
				doShoot = 10;
			}
			// jump
			else{
				if(!falling)
					jump = true;
			}

		}
		break;
	}
}
void Hero::EventListener( SDL_Event *event )
{
	switch(event->type)
	{
	case SDL_KEYDOWN:
		{
			if((event->key.keysym.sym == SDLK_UP &&	type == FIRSTPLAYER) ||
				(event->key.keysym.sym == SDLK_w && type == SECONDPLAYER))
			{
				if(!falling){
					jump = true;
					jumpPower = 10;
				}
			}
		}
		switch(event->key.keysym.sym)
		{

			break;
		}
		break;
	case SDL_KEYUP:
		{
			switch(event->key.keysym.sym)
			{
			case SDLK_UP:
				//	jump = false;
				break;
			}
		}
		break;
	}
}

void Hero::Init()
{
	if(type == FIRSTPLAYER)
	{
		body->SetTransform(b2Vec2(150.0f,650.0f),0);
		stick->SetTransform(body->GetPosition(),1.849094);
	}
	else if(type == SECONDPLAYER)
	{
		body->SetTransform(b2Vec2(850.0f,650.0f),0);
		stick->SetTransform(body->GetPosition(),1.292507);
	}
	body->SetLinearVelocity(b2Vec2(0,0));
	body->SetAngularVelocity(0);

}

void Hero::GhostMode(bool onoff)
{
	ghost = onoff;
	if(onoff == true)
	{
		tristana.groupIndex = -2;
		tristana.categoryBits = 0x0002;
		tristana.maskBits = 0x0004;
		body->GetFixtureList()->SetFilterData(tristana);

		pantheon.categoryBits = 0x0002;
		pantheon.maskBits = 0x0002;
		stick->GetFixtureList()->SetFilterData(pantheon);
	}
	else
	{
		tristana.groupIndex = 2;
		body->GetFixtureList()->SetFilterData(tristana);

		pantheon.categoryBits = 1;
		pantheon.maskBits = 65535;
		stick->GetFixtureList()->SetFilterData(pantheon);
	}
}




void Hero::MoveLeft()
{
	if(OnSpeed == false)
	{
		body->SetLinearVelocity(b2Vec2(-30.0f + GameScene::GetWindPower(),body->GetLinearVelocity().y));
	}
	else
	{
		body->SetLinearVelocity(b2Vec2(-50.0f + GameScene::GetWindPower(),body->GetLinearVelocity().y));
	}
}

void Hero::MoveRight()
{
	if(OnSpeed == false)
	{
		body->SetLinearVelocity(b2Vec2(30.0f,body->GetLinearVelocity().y));

	}
	else
	{
		body->SetLinearVelocity(b2Vec2(50.0f,body->GetLinearVelocity().y));

	}

}

void Hero::SetType( int _type )
{
	PlayerType = _type;
	if(_type == WHITE)
		name_texture = "PLAYER_WHITE";
	else if(_type == BLACK)
		name_texture = "PLAYER_BLACK";
	else if(_type == YELLOW)
		name_texture = "PLAYER_YELLOW";
}


