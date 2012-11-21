#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "sdl_gdiplus.h"
#include <iostream>

ResourceManager* ResourceManager::Instance = NULL;
std::map<string, SDL_Texture*> *ResourceManager::TextureMap;
//SDL_Surface *ResourceManager::screen;

ResourceManager::ResourceManager()
{
	TextureMap = new std::map<std::string, SDL_Texture*>();
}

ResourceManager::~ResourceManager()
{
	if(TextureMap)
	{
		TextureMap->clear();
		delete TextureMap;
		TextureMap = NULL;
	}
}
void ResourceManager::PushTexture( string name, const char *filename )
{
	SDL_Surface *surface ;
	surface = IMG_Load(filename);
	if(!surface )
	{
		if(MessageBox(NULL,L"이미지로드 실패요",L"ㅋ",MB_OK) == IDOK)
			exit(1);
	}

	(*TextureMap)[name] =  SDL_CreateTextureFromSurface(GameManager::renderer,surface);
}

void ResourceManager::PopTexture( string name )
{
	if(TextureMap->find(name) != TextureMap->end())
	{
		//glDeleteTextures(1,&(*TextureMap)[name]);
	
	//	SDL_FreeSurface((*TextureMap)[name]);
		TextureMap->erase(name);	
	}
}

void ResourceManager::Draw( string name, SDL_Rect Position,SDL_Rect Frame )
{
	//SDL_QueryTexture((*TextureMap)[name],NULL,NULL,&Position.w,&Position.h);
	//Position.w /= (Position.w/Frame.w);	 
	//Position.h /= (Position.h/Frame.h);
	SDL_RenderCopy(GameManager::renderer,(*TextureMap)[name],&Frame,&Position);

	// position x,y 는 위치 w,h 는 게임이미지크기
	// frame x,y 프레임위치 v프레임크기
}

void ResourceManager::Draw( string name,int x,int y ,SDL_RendererFlip flip)
{
	SDL_Rect Pos;
	Pos.x = x;
	Pos.y = y;
	SDL_QueryTexture((*TextureMap)[name],NULL,NULL,&Pos.w,&Pos.h);
	SDL_RenderCopyEx(GameManager::renderer,(*TextureMap)[name],NULL,&Pos,0,NULL,flip);
}

void ResourceManager::Draw( string name,SDL_Rect Position,SDL_Rect Frame,double angle,SDL_RendererFlip flip )
{
	//SDL_QueryTexture((*TextureMap)[name],NULL,NULL,&Position.w,&Position.h);
	//Position.w /= (Position.w/Frame.w);	 
	//Position.h /= (Position.h/Frame.h);
	SDL_RenderCopyEx(GameManager::renderer,(*TextureMap)[name],&Frame,&Position,angle,NULL,flip);
}

SDL_Texture* ResourceManager::getTexture(string name){
	return (*TextureMap)[name];
}
