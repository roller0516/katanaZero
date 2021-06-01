#include "EnemyManager.h"
#include "Enemy.h"
#include "Enemy_Bold.h"
#include "Enemy_pomp.h"
#include "Enemy_Grunt.h"
#include "Enemy_Cop.h"
#include "Player.h"
#include "AstarManager.h"
HRESULT EnemyManager::Init(Player* player,int x, int y,int index)
{
    vEnemyList[index]->Init(x,y);
    vEnemyList[index]->GetData()->target = player;
    vEnemyList[index]->GetData()->Index = index;
    return S_OK;
}

void EnemyManager::Update()
{
    for (int i = 0; i < vEnemyList.size(); i++)
    {
        vEnemyList[i]->Update();
    }
}

void EnemyManager::Release()
{
    map<string, Enemy*>::iterator it;
    for (it = mEnemyList.begin(); it != mEnemyList.end(); it++)
    {
        if ((it->second))
        {
            (it->second)->Release();
            delete (it->second);
            (it->second) = nullptr;
        }
    }
    mEnemyList.clear();

}

void EnemyManager::Render(HDC hdc)
{
    for (int i = 0; i < vEnemyList.size(); i++)
    {
        vEnemyList[i]->Render(hdc,false);
    }
}

void EnemyManager::AddEnemy(string name,int size)
{
    for (int i = 0; i < size; i++) 
    {
        vEnemyList.push_back(CreateClone(name));
    }
}

void EnemyManager::DeletEnemy(int index)
{
    vEnemyList[index]->Release();
    delete vEnemyList[index];
    vEnemyList[index] = nullptr;
    vEnemyList.erase(vEnemyList.begin() + index);
}

Enemy* EnemyManager::CreateClone(string name)
{
    return FindClone(name)->Clone();
}

Enemy* EnemyManager::FindClone(string name)
{
    map<string, Enemy*>::iterator it;
    it = mEnemyList.find(name);

    if (it == mEnemyList.end())
    {
        return nullptr;
    }

    return it->second;
}
void EnemyManager::RegisterClone(string name ,Enemy* clone)
{
    mEnemyList.insert(make_pair(name,clone));
}







