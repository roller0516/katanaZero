#include "Missile.h"
#include "Enemy.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Camera.h"

HRESULT Missile::Init(Enemy* owner)
{
	this->owner = owner;

	worldPos = {-100, -100};
	shape = { 0, 0, 0, 0 };
	moveSpeed = 800.0f;
	moveTime = 10.0f;
	size = 50;
	angle = 0.0f;
	isFired = false;
	fireStep = 0;
	destAngle = 0.0f;

	// ÀÌ¹ÌÁö
	img = ImageManager::GetSingleton()->AddImage("Bullet", "Image/Katana/enemy/enemy_bullet.bmp", 48, 2,1,1,true, RGB(255,0,255));
    return S_OK;
}

void Missile::Release()
{

}

void Missile::Update()
{
	if (isFired)
	{
		MovingNormal();

		if (localPos.x < 0 || localPos.y < 0 || localPos.x > WINSIZE_X || localPos.y > WINSIZE_Y)
		{
			isFired = false;
		}
		
	}

	shape.left = localPos.x - size / 2;
	shape.top = localPos.y - 1;
	shape.right = localPos.x + size / 2;
	shape.bottom = localPos.y + 1;
}

void Missile::Render(HDC hdc)
{
	if (isFired)
	{
		localPos.x = worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
		localPos.y = worldPos.y - Camera::GetSingleton()->GetCameraPos().y;

		if (angle >=0 && angle <DegToRad(10) || angle > DegToRad(170)&& angle <= DegToRad(180))
			img->FrameRender(hdc, localPos.x, localPos.y, 0,0, true, 1.3f);
		else
			img->rotateRender(hdc, localPos.x, localPos.y,0,0,angle);
	}
}

void Missile::MovingNormal()
{
	float elapsedTime = TimerManager::GetSingleton()->GetElapsedTime();
	worldPos.x += cosf(angle) * moveSpeed * elapsedTime;
	worldPos.y -= sinf(angle) * moveSpeed * elapsedTime;
}

void Missile::SetIsFired(bool isFired)
{
	this->isFired = isFired;
}
