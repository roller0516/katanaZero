#include "Enemy_Grunt.h"
#include "Camera.h"
#include "Image.h"
HRESULT Enemy_Grunt::Init(int posX, int posY)
{
	image = ImageManager::GetSingleton()->AddImage("Grunt_Idle", "Image/Katana/enemy/enemy_grunt_idle_8x2.bmp", 480, 144, 8, 2, true, RGB(255, 0, 255));
    data->worldPos.x = posX;
    data->worldPos.y = posY;
    data->isAlive = true;
    data->maxFrame = 8;
	return S_OK;
}

void Enemy_Grunt::Release()
{
}

void Enemy_Grunt::Update()
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

void Enemy_Grunt::Render(HDC hdc, bool world)
{
    if (data->isAlive)
    {
        if (world)
        {
            if (image)
                image->FrameRender(hdc, data->worldPos.x, data->worldPos.y, data->currFrameX, 0, true);
        }
        else
            image->FrameRender(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
    }
}

Enemy* Enemy_Grunt::Clone()
{
	return new Enemy_Grunt;
}

void Enemy_Grunt::Pattern()
{
}

MissileManager* Enemy_Grunt::GetMissileManager()
{
    return nullptr;
}
