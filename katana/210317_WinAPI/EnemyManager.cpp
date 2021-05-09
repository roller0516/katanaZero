#include "EnemyManager.h"
#include "Enemy.h"

HRESULT EnemyManager::Init()
{
    // 1) 배열
    //Enemy* enemys = new Enemy[10];
    //for (int i = 0; i < 10; i++)
    //{
    //    enemys[i].Init();
    //}

    // TODO : emplace_back() 성능 차이
    // 2) vector push_back()
    //for (int i = 0; i < 10; i++)
    //{
    //    vEnemys.push_back(new Enemy());
    //    vEnemys[i]->Init();
    //}

    // 3) vector resize()
    vEnemys.resize(10);
    for (int i = 0; i < 10; i++)
    {
        //vEnemys.push_back(new Enemy());
        vEnemys[i] = new Enemy();
        vEnemys[i]->Init(150 + (i % 5) * 200, 100 + (i / 5) * 500);
    }


    //// 4) vector reserve()
    //vEnemys.reserve(10);
    //for (int i = 0; i < 10; i++)
    //{
    //    vEnemys.push_back(new Enemy());
    //    vEnemys[i]->Init();
    //}


    return S_OK;
}

void EnemyManager::Release()
{
    // 반복자(iterator) : STL 자료구조를 구성하는 원소의 메모리를 저장하는 객체
    vector<Enemy*>::iterator it;
    for (it = vEnemys.begin(); it != vEnemys.end(); it++)
    {
        (*it)->Release();   // (*it) -> Enemy* 데이터타입확인
        delete (*it);
        (*it) = nullptr;
    }
}

void EnemyManager::Update()
{
    vector<Enemy*>::iterator it;
    for (it = vEnemys.begin(); it != vEnemys.end(); it++)
    {
        (*it)->Update();
    }
}

void EnemyManager::Render(HDC hdc)
{
    vector<Enemy*>::iterator it;
    for (it = vEnemys.begin(); it != vEnemys.end(); it++)
    {
        (*it)->Render(hdc);
    }
}

void EnemyManager::AddEnemy(int size)
{
    for (int i = 0; i < size; i++)
    {
        vEnemys.push_back(new Enemy());
        vEnemys[vEnemys.size() - 1]->Init();
    }
}
