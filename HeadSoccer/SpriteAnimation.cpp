
#include "SpriteAnimation.h"

#include "ResourceManager.h"

#include <sdl.h>
#include <sdl_gdiplus.h>

SpriteAnimation::SpriteAnimation(string name,int w,int h)
{
	res = name;
	this->w = w;
	this->h = h;
	this->step = 0;
}

SpriteAnimation::~SpriteAnimation(void)
{
}

void SpriteAnimation::draw(int x,int y,float angle){
	SDL_Rect pos = {x,y,0,0};
	SDL_QueryTexture(ResourceManager::getTexture(res),NULL,NULL,&pos.w,&pos.h);

	pos.w /= w;
	pos.h /= h;

	SDL_Rect src = {pos.w * (step / w),
					pos.h * (step % w),
					pos.w,
					pos.h};
	ResourceManager::getInstance()->Draw(res,pos,src,angle,SDL_FLIP_NONE);
}

void SpriteAnimation::doStep(){
	step ++;
	if(step >= w*h)
		step = 0;
}
