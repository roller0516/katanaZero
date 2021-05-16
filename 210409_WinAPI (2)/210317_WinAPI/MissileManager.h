#pragma once
#include "GameNode.h"


class Missile;
class MissileManager :public GameNode
{
public:
    enum MISSILETYPE
    {
        PATTERN1,
        PATTERN2,
        END
    };
private:
    vector<Missile*> vMissile;
    float angle;

    int count;
    int elapsedTime;
    int timer;

    bool isSkillon;
public:
    MISSILETYPE missile;
    HRESULT Init();
    void Release();
    void Update();
    void Render(HDC hdc);
    void FireSkill_01(FPOINT enemyPos);
    void FireSkill_02(FPOINT enemyPos);
    void SetPos(FPOINT enemyPos);
    void SetFired(bool fired);
    bool GetIsSkillon() { return this->isSkillon; }
};

