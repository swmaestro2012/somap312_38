#pragma once
#include <iostream>
#include "SDL.h"
#include "sdl_gdiplus.h"
#include <Box2D/Box2D.h>
using namespace std;
class Object
{
private:


protected:
	Object(void);
	~Object(void);

	SDL_Rect Position;
	SDL_Rect Frame; //�̹��� �������� �κ�
	float angle;
	SDL_RendererFlip flip;

	
	void SetPosition(float x,float y);
	//void LoadTexture(char *fiename);
	//void Draw();
public:
	SDL_Rect GetPos();
};
