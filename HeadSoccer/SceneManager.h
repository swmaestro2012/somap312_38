#pragma once
#include "Scene.h"
#include <map>
#include <string>
#include <SDL.h>
using namespace std;
class SceneManager
{
private:
	static map<string, Scene*> * SceneMap;
	static Scene* NowScene;
	void Init();
public:
	SceneManager();
	~SceneManager();

	static void PushScene(string name, Scene* scene); // Scene을 Map에 저장
	static void PopScene(string name); // name에 해당하는 씬을 찾고 제거
	static void ChangeScene(string name); // NowScene에 name에 해당하는 Scene이 있는지 검색 후 있을 시 대입

	void Render();
	void Update(float dt);
	void EventListener(SDL_Event *event);
};