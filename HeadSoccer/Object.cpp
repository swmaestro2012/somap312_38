#include "Object.h"
#include "GameManager.h"
Object::Object(void)
{
	Position.x = 0;
	Position.y = 0;
	angle = 0.f;
	flip = SDL_FLIP_NONE;
}

Object::~Object(void)
{
}

SDL_Rect Object::GetPos()
{
	return Position;
}

//void Object::Draw()
//{
//	//SDL_QueryTexture(Texture,NULL,NULL,&Position.w,&Position.h);
//	//Position.w /= (Position.w/Frame.w);	 
//	//Position.h /= (Position.h/Frame.h);
//	SDL_RenderCopyEx(GameManager::renderer,Texture,&Frame,&Position,angle,NULL,flip);
////	ResourceManager::getInstance()->Draw(name,Position,Frame);
//}
//
//void Object::LoadTexture(char *filename)
//{	
//	SDL_Surface *surface ;
//	surface = IMG_Load(filename);
//	Texture=  SDL_CreateTextureFromSurface(GameManager::renderer,surface);
//
//}

void Object::SetPosition( float x,float y )
{
	Position.x = x;
	Position.y = y;
}

