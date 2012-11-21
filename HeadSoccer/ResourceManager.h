#pragma once

#include <map>
#include <Box2D\Box2D.h>
#include <string>
#include "SDL.h"
#include "sdl_gdiplus.h"
#include "GameManager.h"
using namespace std;
class ResourceManager
{
private:
	static ResourceManager* Instance;
	static std::map<string, SDL_Texture*> *TextureMap;
private:
	ResourceManager();
	~ResourceManager();
public:
	static ResourceManager* getInstance() { if(!Instance) Instance = new ResourceManager(); return Instance; }

	static void destroyInstance() { if(Instance) delete Instance; }

	static void PushTexture(string name,  const char* filename); // µ¤¾î¾º¿ò
	static void PopTexture(string name); // °Ë»ö ÈÄ »èÁ¦
	static void Draw(string name, SDL_Rect Position,SDL_Rect Frame);
	static void Draw(string name,int x,int y,SDL_RendererFlip flip);
	static void Draw(string name,SDL_Rect Position,SDL_Rect Frame,double angle,SDL_RendererFlip flip);


	static SDL_Texture *ResourceManager::getTexture(string name);
};