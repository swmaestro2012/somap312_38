#pragma once

#include <string>
using namespace std;

class SpriteAnimation
{
public:
	SpriteAnimation(string name,int w,int h);
	~SpriteAnimation(void);


	void draw(int x,int y,float angle=0);
	void doStep();

	int step;

private:
	string res;
	int w,h;
};
