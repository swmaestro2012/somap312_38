#include "WContactListener.h"
#include "Ball.h"
WContactListener::WContactListener(void)
{
}

WContactListener::~WContactListener(void)
{
}

void WContactListener::BeginContact( b2Contact* contact )
{
	if((contact->GetFixtureA()->GetUserData() == "stick_first" && contact->GetFixtureB()->GetUserData() == "ball") ||
		(contact->GetFixtureA()->GetUserData() == "ball" && contact->GetFixtureB()->GetUserData() == "stick_first") ||
		(contact->GetFixtureA()->GetUserData() == "stick_second" && contact->GetFixtureB()->GetUserData() == "ball") ||
		(contact->GetFixtureA()->GetUserData() == "ball" && contact->GetFixtureB()->GetUserData() == "stick_second") )
	{
		if(contact->GetFixtureA()->GetUserData() == "stick_first" ||
			contact->GetFixtureB()->GetUserData() == "stick_first")
			Ball::state = FIRSTPLAYER;

		if(contact->GetFixtureA()->GetUserData() == "stick_second"||
			contact->GetFixtureB()->GetUserData() == "stick_second")
			Ball::state = SECONDPLAYER;
	}

	if( (contact->GetFixtureA()->GetUserData() == "LEFTWALL" && contact->GetFixtureB()->GetUserData() == "ball") ||
		(contact->GetFixtureA()->GetUserData() == "ball" && contact->GetFixtureB()->GetUserData() == "LEFTWALL"))
	{
		if(contact->GetFixtureA()->GetUserData() == "ball")
			contact->GetFixtureA()->GetBody()->SetLinearVelocity(b2Vec2(50.f,0.f));
		else
			contact->GetFixtureB()->GetBody()->SetLinearVelocity(b2Vec2(50.f,0.f));
	}

	if( (contact->GetFixtureA()->GetUserData() == "RIGHTWALL" && contact->GetFixtureB()->GetUserData() == "ball") ||
		(contact->GetFixtureA()->GetUserData() == "ball" && contact->GetFixtureB()->GetUserData() == "RIGHTWALL"))
	{
		if(contact->GetFixtureA()->GetUserData() == "ball")
			contact->GetFixtureA()->GetBody()->SetLinearVelocity(b2Vec2(50.f,0.f));
		else
			contact->GetFixtureB()->GetBody()->SetLinearVelocity(b2Vec2(50.f,0.f));
	}
}

void WContactListener::EndContact( b2Contact* contact )
{

}
