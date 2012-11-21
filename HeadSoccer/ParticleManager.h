#pragma once

#ifndef WOONG_H
#define WOONG_H

#include "Particle.h"

#include <vector>
using namespace std;

class ParticleManager
{
public:
	ParticleManager(void);
	~ParticleManager(void);

	static void add(int x,int y,float sx,float sy,int color = 0);

	static void update();
	static void draw();


private:
	static vector<Particle> particles;
};

#endif //WOONG_H
