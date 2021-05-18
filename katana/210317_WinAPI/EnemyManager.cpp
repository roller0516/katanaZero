#include "EnemyManager.h"
#include "Enemy.h"
#include "Enemy_Bold.h"
#include "Enemy_pomp.h"
#include "Enemy_Grunt.h"
#include "Enemy_Cop.h"

HRESULT EnemyManager::Init()
{
    return S_OK;
}

void EnemyManager::Update()
{
    for (int i = 1; i < mEnemyList.size(); i++)
    {
        mEnemyList[i]->Update();
    }
}

void EnemyManager::Release()
{
    
}

void EnemyManager::Render(HDC hdc)
{
    for (int i = 0; i < mEnemyList.size(); i++)
    {
        mEnemyList[i]->Render(hdc);
    }
}

Enemy* EnemyManager::CreateClone(Enemy* clone)
{
    return clone->Clone();
}

void EnemyManager::RegisterClone(Enemy* clone, int x, int y)
{
    map<Type, Enemy*>::iterator iter;
    for (iter = mEnemyList.begin(); iter != mEnemyList.end(); iter++) 
    {
       if ((*iter).first == Type::Pomp)
       {
         mEnemyList[type] = CreateClone(clone);
         mEnemyList[type]->Init(x, y);
       }
    }
   
    
}







