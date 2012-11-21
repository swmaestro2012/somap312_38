#pragma once

#ifndef WOONGPARTICLE_H
#define WOONGPARTICLE_H

#include "SpriteAnimation.h"


#define RED 1
#define BLUE 0

class Particle
{
public:
	Particle(int x,int y,float sx,float sy,int color=0);
	~Particle(void);

	void update();
	void draw();

	float x,y;
	float sx,sy;
	float angle;
	int step;
	int color;

	bool dead;
	float alpha;

	static SpriteAnimation *sprite;
		static SpriteAnimation *spriteRed;
};

#endif //WOONGPARTICLE_H