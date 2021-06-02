#include "MissileManager.h"
#include "Missile.h"

HRESULT MissileManager::Init()
{
    //this->owner = owner;

    vMissiles.resize(50);
    vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        (*it) = new Missile();
        (*it)->Init();
    }
    return S_OK;
}

void MissileManager::Release()
{
    vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        (*it)->Release();
        delete (*it);
        (*it) = nullptr;
    }
    vMissiles.clear();
}

void MissileManager::Update()
{
    for (int i = 0; i < vMissiles.size(); i++)
    {
        vMissiles[i]->Update();
    }
}

void MissileManager::Render(HDC hdc)
{
    for (int i = 0; i < vMissiles.size(); i++)
    {
        vMissiles[i]->Render(hdc);
    }
}

void MissileManager::Fire(float angle,int x, int y, float delay)
{
    vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        if ((*it)->GetIsFired() == false)
        {
            (*it)->SetOwnerType(MissileOwnerType::enemy);
            (*it)->SetType(MissileType::Normal);
            (*it)->SetAngle(angle);
            (*it)->SetPos(x, y);
            (*it)->SetIsFired(true);
            (*it)->SetDelay(delay);
            break;
        }
    }
}

void MissileManager::Lazer(float angle, int x, int y,float delay)
{
    vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        if ((*it)->GetIsFired() == false)
        {
            (*it)->SetType(MissileType::lazer);
            (*it)->SetOwnerType(MissileOwnerType::enemy);
            (*it)->SetAngle(angle);
            (*it)->SetPos(x, y);
            (*it)->SetIsFired(true);
            (*it)->SetDelay(delay);
            break;
        }
    }
}

void MissileManager::TopLazer(float angle, int x, int y, float delay)
{
    vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        if ((*it)->GetIsFired() == false)
        {
            (*it)->SetType(MissileType::Toplazer);
            (*it)->SetAngle(angle);
            (*it)->SetPos(x, y);
            (*it)->SetDelay(delay);
            (*it)->SetIsFired(true);
            (*it)->SetOwnerType(MissileOwnerType::enemy);
            break;
        }
    }
}

void MissileManager::Mine(float angle, int x, int y, float delay)
{
    vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        if ((*it)->GetIsFired() == false)
        {
            (*it)->SetAngle(angle);
            (*it)->SetPos(x, y);
            (*it)->SetIsFired(true);
            (*it)->SetDelay(delay);
            (*it)->SetOwnerType(MissileOwnerType::enemy);
            (*it)->SetType(MissileType::Mine);
            break;
        }
    }
}
