#pragma once
#include "GameNode.h"
class Image;
class Enemy;
class Player;
class BossScene :
    public GameNode
{
private:
    Player* player;
    Enemy* boss;
    Image* curSor;
public:
    virtual HRESULT Init();
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    void MapLoad(int stageNum);

    virtual ~BossScene() {};
};

