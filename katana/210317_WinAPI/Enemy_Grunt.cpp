#include "Enemy_Grunt.h"
#include "Camera.h"
#include "Image.h"
HRESULT Enemy_Grunt::Init(int posX, int posY)
{
	image = ImageManager::GetSingleton()->AddImage("Grunt_Idle", "Image/Katana/enemy/enemy_grunt_idle_8x2.bmp", 480, 144, 8, 2, true, RGB(255, 0, 255));
    worldPos.x = posX;
    worldPos.y = posY;
    isAlive = true;
    maxFrame = 8;
	return S_OK;
}

void Enemy_Grunt::Release()
{
}

void Enemy_Grunt::Update()
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

void Enemy_Grunt::Render(HDC hdc, bool world)
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

Enemy* Enemy_Grunt::Clone()
{
	return new Enemy_Grunt;
}

void Enemy_Grunt::Pattern()
{
}
