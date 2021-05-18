#pragma once
#include "GameNode.h"
#include <vector>

class Enemy;
class EnemyManager : public GameNode
{
public:
	enum Type
	{
		Pomp,
		Grnt,
		End
	};
private:
	map<Type,Enemy*> mEnemyList;
	Type type;
public:
	HRESULT Init();
	void Update();
	void Release();
	void Render(HDC hdc);
	Enemy* CreateClone(Enemy* clone);
	void RegisterClone(Enemy* clone, int x = 0, int y = 0);
	inline int GetClone(int index){return index;};
};

