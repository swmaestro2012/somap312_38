#pragma once
#include <SDL.h>
class Scene
{
private:
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Init() = 0;
	virtual void Render() = 0;
	virtual void Update(float dt) = 0;
	virtual void EventListener(SDL_Event *event) = 0;
};