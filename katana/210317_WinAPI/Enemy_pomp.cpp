#include "Enemy_pomp.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Player.h"

HRESULT Enemy_pomp::Init(int posX, int posY)
{
    ImageManager::GetSingleton()->AddImage("Pomp_Idle", "enemy_pomp_idle_8x2", 528, 168, 8, 2, true, RGB(255, 0, 255));
    currFrameX = 0;
    updateCount = 0;

    pos.x =  posX;
    pos.y = posY;
    size = 80;
    name = "NormalEnemy";
    shape = { 0, 0, 0, 0 };
    moveSpeed = 3.3f;
    isAlive = true;
    angle = 0.0f;
    target = nullptr;
    maxFrame = 8;
    return S_OK;
}

void Enemy_pomp::Release()
{
}

void Enemy_pomp::Update()
{
    if (isAlive)
    {
        currFrameX += TimerManager::GetSingleton()->GetElapsedTime() * 10;
        if (currFrameX >= maxFrame)
            currFrameX = 0;
    }
}

void Enemy_pomp::Render(HDC hdc)
{
    if (isAlive)
    {
        if (image)
        {
            image->FrameRender(hdc, pos.x, pos.y, currFrameX, 0, true);
        }
    }
}

Enemy* Enemy_pomp::Clone()
{
    return new Enemy_pomp();
}

void Enemy_pomp::Move()
{
   
}

void Enemy_pomp::HorizonMove()
{
   
}
