#include "Item.h"
#include <time.h>
#include <stdlib.h>


Item::Item()
{
	srand((unsigned int)time(NULL));
	Position.x = rand()%500 + 200;//700.0f;
	Position.y = rand()%250 + 400;//500.0f;
	Frame.w = Frame.h =	Position.w = Position.h = 50;
	Frame.x = Frame.y = 0;
	type =rand()%3;
	state = SHOW;
	StopWatch = 0.f;
	rightwind = rand()%2;
	back_frame.x = back_frame.y = 0;
	back_frame.w = back_frame.h = back_pos.w = back_pos.h = 75;
	right = rand()%2;
	backangle = 0.f;
}

Item::~Item(void)
{
}


void Item::Update()
{
	backangle += 1.f;
	if(right == true)
	{
		Position.x += 1.f;
	}
	else
	{
		Position.x -= 1.f;
	}

	if(Position.x < 200.f)
		right = true;
	else if(Position.x > 800.f)
		right = false;
}

void Item::Render()
{
	back_pos.x = Position.x - 12.f;
	back_pos.y = Position.y - 12.f;
	if(state == SHOW)
	{
		ResourceManager::getInstance()->Draw("ITEMBACK",back_pos,back_frame,backangle,flip);
		switch(type)
		{
		case FAST:
			ResourceManager::getInstance()->Draw("FAST",Position,Frame,angle,flip);
			break;
		case WIND:
			ResourceManager::getInstance()->Draw("WIND",Position,Frame,angle,flip);
			break;
		case GHOST:
			ResourceManager::getInstance()->Draw("GHOST",Position,Frame,angle,flip);
			break;
		case BOUNCE:
			ResourceManager::getInstance()->Draw("BOUNCE",Position,Frame,angle,flip);
			break;
		}
	}

}

SDL_Point Item::GetCenterPos()
{
	SDL_Point pantheon;
	pantheon.x = Position.x + Position.w /2;
	pantheon.y = Position.y + Position.h /2;

	return pantheon;
}

void Item::Using(float dt)
{
	StopWatch += dt;
	if(StopWatch > 3.f)
		state = DIE;
}

