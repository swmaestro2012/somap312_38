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

	static void PushScene(string name, Scene* scene); // Scene�� Map�� ����
	static void PopScene(string name); // name�� �ش��ϴ� ���� ã�� ����
	static void ChangeScene(string name); // NowScene�� name�� �ش��ϴ� Scene�� �ִ��� �˻� �� ���� �� ����

	void Render();
	void Update(float dt);
	void EventListener(SDL_Event *event);
};