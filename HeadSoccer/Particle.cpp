#include "Particle.h"

#include "ResourceManager.h"
#include <sdl.h>

SpriteAnimation *Particle::sprite = new SpriteAnimation("BlueParticle",4,4);
SpriteAnimation *Particle::spriteRed = new SpriteAnimation("RedParticle",4,4);

Particle::Particle(int x,int y,float sx,float sy,int color)
{
	this->x = x;
	this->y = y;
	this->sx = sx;
	this->sy = sy;

	this->step = 0;
	this->color = color;
	//printf("%d %d %d %d\n", (int)this->x,(int)this->y,(int)this->sx,(int)this->sy);

	this->dead = false;
	this->alpha = 255;
}

Particle::~Particle(void)
{
}


void Particle::update(){
	x += sx;
	y += sy;

	alpha -= 2;
	angle += 10;

	if(alpha <= 0)
		dead = true;
}
void Particle::draw(){
	sprite->step = alpha / 16;
	switch(color){
		case BLUE:
			SDL_SetTextureAlphaMod(ResourceManager::getTexture("BlueParticle"),alpha);
			sprite->draw(x,y,angle);
			break;
		case RED:
			SDL_SetTextureAlphaMod(ResourceManager::getTexture("RedParticle"),alpha);
			spriteRed->draw(x,y,angle);
			break;
	}
	
	
}