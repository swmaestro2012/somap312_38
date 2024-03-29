#include "SDL.h"
#include "sdl_gdiplus.h"
#include "GameManager.h"
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
SDL_Texture *loadTexture(SDL_Renderer *renderer,const char *path){

	SDL_Texture *tex;
	SDL_Surface *surface;

	surface = IMG_Load(path);

	tex = SDL_CreateTextureFromSurface(renderer,surface);

	SDL_FreeSurface(surface);
	return tex;

}
void drawTexture(SDL_Renderer *renderer,SDL_Texture *tex,int x,int y){
	SDL_Rect dstRect = {x,y};
	SDL_QueryTexture(tex,NULL,NULL,&dstRect.w,&dstRect.h);
	SDL_RenderCopy(renderer,tex,NULL,&dstRect);
}

int main(int a,char **b)
{
	SDL_Init(SDL_INIT_VIDEO);

	GameManager gameManager;
	gameManager.MainLoop();

	/*SDL_Window *window;
	SDL_Renderer *renderer;

	window = SDL_CreateWindow("woong",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
		640,480,SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window,-1,0);

	SDL_Texture *texture;
	
	texture = loadTexture(renderer,"background1.png");

	SDL_Event event;
	bool quit = false;
	while(!quit){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					quit = true;
					break;
			}
		}

		{
			SDL_RenderClear(renderer);


			drawTexture(renderer,texture,0,0);


			SDL_RenderPresent(renderer);
		}
		
		SDL_Delay(1);
	}

	SDL_Quit();*/

	return 0;
}
