#include "MissileManager.h"
#include "Missile.h"
HRESULT MissileManager::Init()
{
    for (int i = 0; i < 100; i++) 
    {
        vMissile.push_back(new Missile());
        vMissile[i]->Init();
    }
    angle = 0;
    timer = 0;
    return S_OK;
}

void MissileManager::Release()
{
    for (int i = 0; i < 100; i++)
    {
        vMissile[i]->Release();
    }
    vMissile.clear();
}

void MissileManager::Update()
{
    
    for (int i = 0; i < 100; i++)
    {
        vMissile[i]->Update();
    }

    if (isSkillon) 
    {
        elapsedTime++;
        if (elapsedTime >= 5)
        {
            vMissile[timer]->SetDraw(true);
            if (timer >= 50)
            {
                for (int j = 0; j < 50; j++)
                {
                     vMissile[j]->SetType(vMissile[j]->StarMoving);
                     vMissile[j]->SetIsFired(true);
                }
            }
            timer++;
            elapsedTime = 0;
        }
    }
    if (vMissile[39]->GetPos().x < 0 || vMissile[39]->GetPos().y < 0 || 
        vMissile[39]->GetPos().x > WINSIZE_X || vMissile[39]->GetPos().y > WINSIZE_Y)
    {
        for(int i= 0 ; i < 50 ; i ++)
            vMissile[i]->SetIsFired(false);
        isSkillon = false;
        timer = 0;
    }
}

void MissileManager::Render(HDC hdc)
{
    for (int i = 0; i < 100; i++)
    {
        vMissile[i]->Render(hdc);
    }
}
void MissileManager::FireSkill_01(FPOINT enemyPos)
{
    // 1. 미사일을 36발을 쏘고 일정 높이 이상 올라가면 36방향으로 퍼지게 한다.
    int firedCount = 0;
    for (int i = 0; i < 100; i++)
    {
        if (firedCount < 36 && vMissile[i]->GetIsFired() == false)
        {
            vMissile[i]->SetType(Missile::TYPE::Skill_01);
            vMissile[i]->SetIsFired(true);
            vMissile[i]->SetPos(enemyPos);
            vMissile[i]->SetAngle(angle/* + (firedCount * 0.05f)*/);
            vMissile[i]->SetFireIndex(firedCount);

            firedCount++;
        }
    }

    // 2. 미사일을 한발 쏘고 일정 높이 이상 올라가면 36발을 호출한다.
}

void MissileManager::SetPos(FPOINT enemyPos)
{
    for (int i = 0; i < 100; i++)
    {
        vMissile[i]->SetPos(enemyPos);
    }
}

void MissileManager::SetFired(bool fired)
{
    for (int i = 0; i < 100; i++)
    {
        vMissile[i]->SetIsFired(fired);
    }
}

void MissileManager::FireSkill_02(FPOINT enemyPos)
{
    isSkillon = true;
    int firedCount = 0;
    FPOINT targetPos;
    targetPos.x = enemyPos.x + 125;
    targetPos.y = enemyPos.y;
    for (int i = 0; i < 50; i++)
    {
        if (firedCount < 10 && vMissile[i]->GetIsFired()==false)
        {
            vMissile[i]->SetPos(targetPos.x-=20, targetPos.y+20);
            vMissile[i]->SetAngle(0);
            vMissile[i]->SetFireIndex(firedCount);

            firedCount++; // 0~ 9;
        }
        else if (firedCount >= 10 && firedCount < 20 && vMissile[i]->GetIsFired() == false) //9~ 19
        {
            if (firedCount == 10) 
            {
                targetPos.x = vMissile[9]->GetPos().x;
                targetPos.y = vMissile[9]->GetPos().y;
            }
            vMissile[i]->SetPos(targetPos.x +=cosf(36.0f / 180.0f * 3.14)* 20 , 
                targetPos.y -= sinf(36.0f / 180.0f * 3.14)*20);
            vMissile[i]->SetFireIndex(firedCount);

            firedCount++; // 0~ 9;
        }
        else if (firedCount >= 20 && firedCount < 30 && vMissile[i]->GetIsFired() == false) //9~ 19
        {
            if (firedCount == 20)
            {
                targetPos.x = vMissile[19]->GetPos().x;
                targetPos.y = vMissile[19]->GetPos().y;
            }
            vMissile[i]->SetPos(targetPos.x -= cosf(72.0f / 180.0f * 3.14) * 20,
                targetPos.y += sinf(72.0f / 180.0f * 3.14) * 20);
            vMissile[i]->SetFireIndex(firedCount);

            firedCount++; // 0~ 9;
        }
        else if (firedCount >= 30 && firedCount < 40 && vMissile[i]->GetIsFired() == false) //9~ 19
        {
            if (firedCount == 30)
            {
                targetPos.x = vMissile[29]->GetPos().x;
                targetPos.y = vMissile[29]->GetPos().y;
            }
            vMissile[i]->SetPos(targetPos.x -= cosf(72.0f / 180.0f * 3.14) * 20,
                targetPos.y -= sinf(72.0f / 180.0f * 3.14) * 20);
            vMissile[i]->SetFireIndex(firedCount);

            firedCount++; // 0~ 9;
        }
        else if (firedCount >= 40 && firedCount < 50 && vMissile[i]->GetIsFired() == false) //9~ 19
        {
            if (firedCount == 40)
            {
                targetPos.x = vMissile[39]->GetPos().x;
                targetPos.y = vMissile[39]->GetPos().y;
            }
            vMissile[i]->SetPos(targetPos.x -= cosf(-(144.f / 180.0f * 3.14)) * 20,
                targetPos.y -= sinf(-(144.f / 180.0f * 3.14)) * 20);
            vMissile[i]->SetFireIndex(firedCount);

            firedCount++; // 0~ 9;
        }
    }

}





