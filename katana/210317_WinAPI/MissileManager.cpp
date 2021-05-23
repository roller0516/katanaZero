#include "MissileManager.h"
#include "Missile.h"

HRESULT MissileManager::Init(Enemy* owner)
{
    this->owner = owner;

    vMissiles.resize(10);
    vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        (*it) = new Missile();
        (*it)->Init(this->owner);
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

void MissileManager::Fire(float angle,int x, int y)
{
    vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        if ((*it)->GetIsFired() == false)
        {
            (*it)->SetPos(x, y);
            (*it)->SetIsFired(true);
            (*it)->SetAngle(angle);
            break;
        }
    }
}
