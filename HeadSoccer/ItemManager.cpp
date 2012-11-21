#include "ItemManager.h"
#include <time.h>
#include <stdlib.h>
ItemManager::ItemManager(void)
{
	item_list.clear();
	fast = false;
	wind = false;
	rightwind = false;
	ghost = false;
	bounce = false;
	StopWatch = 0.f;
	srand((unsigned int)time(NULL));
	CreateTime = rand()%5 + 10;
}

ItemManager::~ItemManager(void)
{
} 
 
void ItemManager::Update(float dt,SDL_Point ballCenterPos,float radius)
{
	StopWatch += dt;
	if(StopWatch > CreateTime)
	{
   		item_list.push_back(Item());
		StopWatch = 0.f;
		CreateTime = rand()%5 + 10;
	}
	InitList();
	
	for(item_iter = item_list.begin();item_iter != item_list.end();)
	{
		item_iter->Update();
		if(item_iter->GetState() == Item::SHOW)
		{
			if(radius + item_iter->GetPos().w >	sqrt((float)
				(ballCenterPos.x - item_iter->GetCenterPos().x) * (ballCenterPos.x - item_iter->GetCenterPos().x) +
				(ballCenterPos.y - item_iter->GetCenterPos().y) * (ballCenterPos.y - item_iter->GetCenterPos().y)))
			{
				if(item_iter->GetType() == Item::FAST)
				{
					fastnum = Ball::state;
				}
				else if(item_iter->GetType() == Item::GHOST)
				{
					ghostnum = Ball::state;
				}
				item_iter->SetState(Item::USE);
			}
			item_iter++;
		}
		else if(item_iter->GetState() == Item::USE)
		{
			switch(item_iter->GetType())
			{
			case Item::FAST:
				fast = true;
				break;
			case Item::WIND:
				wind = true;
				rightwind = item_iter->IsRightWind();
				break;
			case Item::GHOST:
				ghost = true;
				break;
			case Item::BOUNCE:
				bounce = true;
				break;
			}
			item_iter->Using(dt);
			item_iter++;
		}
		else if(item_iter->GetState() == Item::DIE)
		{
			item_iter = item_list.erase(item_iter);
		}
		
	}
}

void ItemManager::Render()
{
	for(item_iter = item_list.begin();item_iter != item_list.end();item_iter++)
	{
		item_iter->Render();
	}
}


void ItemManager::InitList()
{
	fast =false;
	wind = false;
	ghost = false;
	bounce = false;
}

void ItemManager::Init()
{
	item_list.clear();
	InitList();
}
