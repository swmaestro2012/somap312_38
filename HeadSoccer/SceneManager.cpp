#include "SceneManager.h"
#include "ResourceManager.h"

map<string, Scene*> * SceneManager::SceneMap = NULL;
Scene* SceneManager::NowScene = NULL;

SceneManager::SceneManager()
{
	SceneMap = new map<string, Scene*>();
}

SceneManager::~SceneManager()	
{
	if(NowScene)
	{
		delete NowScene;
		NowScene = NULL;
	}

	if(SceneMap)
	{
		SceneMap->clear();
		delete SceneMap;
		SceneMap = NULL;
	}
}

void SceneManager::PushScene( string name, Scene* scene )
{
	(*SceneMap)[name] = scene;
}

void SceneManager::PopScene( string name )
{
	if(SceneMap->find(name) != SceneMap->end())
	{
		delete (*SceneMap)[name];
		SceneMap->erase(name);
		
		
	}
}

void SceneManager::ChangeScene( string name )
{
	if(SceneMap->find(name) != SceneMap->end())
	{
		NowScene = (*SceneMap)[name];
		NowScene->Init();
	}
}

void SceneManager::Init()	
{
	if(NowScene)
		NowScene->Init();
}

void SceneManager::Render()
{	
	if(NowScene)
		NowScene->Render();
}

void SceneManager::Update(float dt)
{
	if(NowScene)
		NowScene->Update(dt);
}

void SceneManager::EventListener(SDL_Event *event)
{
	if(NowScene)
		NowScene->EventListener(event);
}
