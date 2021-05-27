#include "Enemy_Cop.h"
#include "Camera.h"
#include "Image.h"
HRESULT Enemy_Cop::Init(int posX, int posY)
{
	image = ImageManager::GetSingleton()->AddImage("Cop_Idle", "Image/Katana/enemy/enemy_cop_idle_8x2.bmp", 576, 148, 8, 2, true, RGB(255, 0, 255));
    worldPos.x = posX;
    worldPos.y = posY;
    isAlive = true;
    maxFrame = 8;
	return S_OK;
}

void Enemy_Cop::Release()
{
}

void Enemy_Cop::Update()
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

void Enemy_Cop::Render(HDC hdc, bool world)
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

Enemy* Enemy_Cop::Clone()
{
	return new Enemy_Cop;
}

void Enemy_Cop::Pattern()
{
}

MissileManager* Enemy_Cop::GetMissileManager()
{
    return nullptr;
}
