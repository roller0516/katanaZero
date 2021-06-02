#pragma once
#include "GameNode.h"
class Image;
class Enemy;
class Player;
class CollisionManager;
class MissileManager;
class UI;
class BossScene :
    public GameNode
{
private:
    CollisionManager* collisionManager;
    MissileManager* missileManager;
    Player* player;
    Enemy* boss;
    Image* curSor;
    UI* ui;
public:
    virtual HRESULT Init();
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    //void MapLoad(int stageNum);

    virtual ~BossScene() {};
};

