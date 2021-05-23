#pragma once
#include "GameNode.h"
#include <vector>

class Player;
class Enemy;
class EnemyManager : public GameNode
{
private:
	vector<Enemy*> vEnemyList;
	map<string,Enemy*> mEnemyList;
	int enemyCount;
public:
	HRESULT Init(Player* player,int x, int y, int index);
	void Update();
	void Release();
	void Render(HDC hdc);
	void AddEnemy(string name,int size);
	void DeletEnemy(int index);
	Enemy* CreateClone(string name);
	Enemy* FindClone(string name);
	void RegisterClone(string name ,Enemy* clone);

	int GetMonsterCount() { return this->enemyCount; }
	void SetMonsterCount(int enemyCount) {this->enemyCount = enemyCount;}
	vector<Enemy*> GetMonsterList() { return this->vEnemyList; }
	void SetEnemyList(vector<Enemy*>& list) { this->vEnemyList = list; }
	inline int GetClone(int index){return index;};
};

