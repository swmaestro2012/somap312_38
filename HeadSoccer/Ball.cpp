#include "Ball.h"

int Ball::state = NULL;
Ball::Ball(void)
{
	Frame.w = Frame.h =	Position.w = Position.h = 50;
	Frame.x = Frame.y = 0;

	b2BodyDef bodyDef;
	bodyDef.position.Set(500.f,100.f);
	bodyDef.type = b2_dynamicBody;

	circleShape.m_radius = 25.0f;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 20.f;
	fixtureDef.restitution = 0.2;
	fixtureDef.friction = 0.9f;
	fixtureDef.userData = "ball";

	fixtureDef.filter.groupIndex = 2;
	ball = GameScene::PhysicsWorld->CreateBody(&bodyDef);
	ball->SetLinearDamping(0.1);
	ball->SetAngularDamping(0.2);
	ball->CreateFixture(&fixtureDef);		
}

Ball::~Ball(void)
{
}

void Ball::Update()
{
	Position.x = ball->GetPosition().x - 25.0f;
	Position.y = ball->GetPosition().y - 25.0f;
	angle = ball->GetAngle()* 180 / 3.14156;
	ball->SetLinearVelocity(b2Vec2(ball->GetLinearVelocity().x + GameScene::GetWindPower(),
	ball->GetLinearVelocity().y));
}

void Ball::Render()
{
	ResourceManager::getInstance()->Draw("BALL",Position,Frame,angle,flip);
}

void Ball::Init()
{
	ball->SetTransform(b2Vec2(500.0f,300.0f),0);
	ball->SetLinearVelocity(b2Vec2(0,0));
	ball->SetAngularVelocity(0);
}

SDL_Point Ball::GetCenterPos()
{
	SDL_Point tristana;
	tristana.x = ball->GetPosition().x;
	tristana.y = ball->GetPosition().y;
	return tristana;
}

void Ball::BounceMode( bool onoff )
{
	if(onoff == true)
		ball->GetFixtureList()->SetRestitution(2.f);
	else
		ball->GetFixtureList()->SetRestitution(0.8f);

}
