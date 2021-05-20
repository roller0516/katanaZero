#include "Enemy_pomp.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Player.h"
#include "Camera.h"

HRESULT Enemy_pomp::Init(int posX, int posY)
{
    image = ImageManager::GetSingleton()->AddImage("Pomp_Idle", "Image/Katana/enemy/enemy_pomp_idle_8x2.bmp", 528, 168, 8, 2, true, RGB(255, 0, 255));
    currFrameX = 0;
    updateCount = 0;
    worldPos.x =  posX;
    worldPos.y = posY;
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
    localPos.x = worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
    localPos.y = worldPos.y - Camera::GetSingleton()->GetCameraPos().y;
    if (isAlive)
    {
        currFrameX += TimerManager::GetSingleton()->GetElapsedTime() * 10;
        if (currFrameX >= maxFrame)
            currFrameX = 0;
    }
}

void Enemy_pomp::Render(HDC hdc,bool world)
{
    if (isAlive)
    {
        if (world) 
        {
            if (image)
                image->FrameRender(hdc, worldPos.x, worldPos.y, currFrameX, 0, true);
        }
        else
            image->FrameRender(hdc, localPos.x, localPos.y, currFrameX, 0, true);
    }
}


Enemy* Enemy_pomp::Clone()
{
    return new Enemy_pomp();
}

void Enemy_pomp::Pattern()
{
}

void Enemy_pomp::Move()
{
   
}

void Enemy_pomp::HorizonMove()
{
   
}
