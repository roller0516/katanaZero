#include "Enemy_pomp.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Player.h"
#include "Camera.h"

HRESULT Enemy_pomp::Init(int posX, int posY)
{
    data = new EnemyData;
    data->image = ImageManager::GetSingleton()->AddImage("Pomp_Idle", "Image/Katana/enemy/enemy_pomp_idle_8x2.bmp", 528, 168, 8, 2, true, RGB(255, 0, 255));
    data->worldPos.x =  posX;
    data->worldPos.y = posY;
    data->size = 80;
    data->Name = "Enemy_pomp";
    data->moveSpeed = 3.3f;
    data->isAlive = true;
    data->maxFrame = 8;
    return S_OK;
}

void Enemy_pomp::Release()
{
}

void Enemy_pomp::Update()
{
    data->localPos.x = data->worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
    data->localPos.y = data->worldPos.y - Camera::GetSingleton()->GetCameraPos().y;
    if (data->isAlive)
    {
        data->currFrameX += TimerManager::GetSingleton()->GetElapsedTime() * 10;
        if (data->currFrameX >= data->maxFrame)
            data->currFrameX = 0;
    }
}

void Enemy_pomp::Render(HDC hdc,bool world)
{
    if (data->isAlive)
    {
        if (world) 
        {
            if (data->image)
                data->image->FrameRender(hdc, data->worldPos.x, data->worldPos.y, data->currFrameX, 0, true);
        }
        else
            data->image->FrameRender(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
    }
}


Enemy* Enemy_pomp::Clone()
{
    return new Enemy_pomp();
}

void Enemy_pomp::Pattern()
{
}

MissileManager* Enemy_pomp::GetMissileManager()
{
    return nullptr;
}

void Enemy_pomp::Move()
{
   
}

void Enemy_pomp::HorizonMove()
{
   
}
