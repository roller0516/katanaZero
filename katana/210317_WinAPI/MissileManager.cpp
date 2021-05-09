#include "MissileManager.h"
#include "Missile.h"

HRESULT MissileManager::Init(Enemy* owner)
{
    this->owner = owner;

    vMissiles.resize(50);
    vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        (*it) = new Missile();
        (*it)->Init(this->owner);

        // 미사일 매니저를 들고 있는 적 객체의 정보를 전달
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

    //vector<Missile*>::iterator it;
    //for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    //{
    //    (*it)->Update();
    //}
}

void MissileManager::Render(HDC hdc)
{
    for (int i = 0; i < vMissiles.size(); i++)
    {
        vMissiles[i]->Render(hdc);
    }
    //vector<Missile*>::iterator it;
    //for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    //{
    //    (*it)->Render(hdc);
    //}
}

void MissileManager::Fire()
{
    vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        if ((*it)->GetIsFired() == false)
        {
            (*it)->SetIsFired(true);
            (*it)->SetAngle(DegToRad (-90));            
            break;
        }
    }
}
