#include "ParticleManager.h"

vector<Particle> ParticleManager::particles;

ParticleManager::ParticleManager(void)
{
}

ParticleManager::~ParticleManager(void)
{
}

void ParticleManager::add(int x,int y,float sx,float sy,int color)
{
	Particle p(x,y,sx,sy,color);

	particles.push_back(p);
}

void ParticleManager::update()
{
	for(int i=0;i<particles.size();i++){
		particles[i].update();

		if(particles[i].dead){
			for(int j=i;j<particles.size()-1;j++){
				particles[j] = particles[j+1];
			}
			particles.pop_back();
		}
	}
}

void ParticleManager::draw()
{
	
	for(int i=0;i<particles.size();i++){
		particles[i].draw();
	}
}

