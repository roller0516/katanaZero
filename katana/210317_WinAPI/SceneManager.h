#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <map>
#include <string>

using namespace std;

class GameNode;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<string, GameNode*>	mSceneDatas;

public:
	static GameNode* currentScene;
	static GameNode* loadingScene;
	static GameNode* readyScene;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	GameNode* AddScene(string key, GameNode* scene);
	HRESULT ChangeScene(string key);

};

