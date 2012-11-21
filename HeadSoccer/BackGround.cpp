#include "BackGround.h"

#include "ParticleManager.h"

BackGround::BackGround(void)
{
	b2FixtureDef fixtureDef;
	fixtureDef.filter.groupIndex = 2;
	groundBodyDef.type = b2_staticBody;
	fixtureDef.filter.categoryBits = 0x0004;
	fixtureDef.filter.maskBits = 0x0002;

	b2EdgeShape edge;

	b2FixtureDef fixtureDef_LR; //ÁÂ¿ìº®
	grounBody = GameScene::PhysicsWorld->CreateBody(&groundBodyDef);
	//À§	
	edge.Set(b2Vec2(0,0),b2Vec2(1024,0));
	grounBody->CreateFixture(&edge,0);
	//¾Æ·¡
	edge.Set(b2Vec2(0,740),b2Vec2(1024,740));
	fixtureDef.shape = &edge;
	grounBody->CreateFixture(&fixtureDef);
	//ÁÂ
	edge.Set(b2Vec2(0,0),b2Vec2(0,768));
	fixtureDef_LR.shape = &edge;
	fixtureDef_LR.userData = "LEFTWALL";
	grounBody->CreateFixture(&fixtureDef_LR);
	//¿ì
	edge.Set(b2Vec2(1024,0),b2Vec2(1024,768));
	fixtureDef_LR.userData = "RIGHTWALL";
	grounBody->CreateFixture(&fixtureDef_LR);
	//¿ÞÂÊ°ñ´ë
	edge.Set(b2Vec2(0,538),b2Vec2(100,538));
	grounBody->CreateFixture(&edge,0);
	//¿À¸¥ÂÊ °ñ´ë
	edge.Set(b2Vec2(924,538),b2Vec2(1024,538));
	grounBody->CreateFixture(&edge,0);
}

BackGround::~BackGround(void)
{
}

void BackGround::Update(int maptype)
{
	switch(maptype)
	{
	case MAP_FIELD:
		for(int i=0;i<rand()%5;i++){
			ParticleManager::add(1024/2,768/2,(rand()%100-50)*0.1,(rand()%30-15)*0.1,1);}
		break;
	case MAP_ROAD:
		for(int i=0;i<rand()%5;i++){
			ParticleManager::add(1024/2,768/2-180,(rand()%100-50)*0.1,(rand()%30-15)*0.1,1);}
		break;
	case MAP_LOCKER:
		for(int i=0;i<rand()%5;i++){
			ParticleManager::add(1024/2,768/2-80,(rand()%100-50)*0.1,(rand()%30-15)*0.1,1);}
		break;
	}
}



void BackGround::Render()
{
	ResourceManager::getInstance()->Draw("BACKGROUND",0,0,SDL_FLIP_NONE);
	ResourceManager::getInstance()->Draw("GROUND",0,738,SDL_FLIP_NONE);
}


void BackGround::Render2(int maptype)
{
	switch(maptype)
	{
	case MAP_FIELD:
		ResourceManager::getInstance()->Draw("LOGO",100,280,SDL_FLIP_NONE);
		ResourceManager::getInstance()->Draw("LOGO",110,290,SDL_FLIP_NONE);
		break;
	case MAP_ROAD:
		ResourceManager::getInstance()->Draw("LOGO",100,100,SDL_FLIP_NONE);
		ResourceManager::getInstance()->Draw("LOGO",110,110,SDL_FLIP_NONE);
		break;
	case MAP_LOCKER:
		ResourceManager::getInstance()->Draw("LOGO",100,200,SDL_FLIP_NONE);
		ResourceManager::getInstance()->Draw("LOGO",110,210,SDL_FLIP_NONE);
		break;
	}
}
