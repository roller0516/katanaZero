#include "Missile.h"
#include "Enemy.h"
#include "CommonFunction.h"
#include "Image.h"

HRESULT Missile::Init(Enemy* owner)
{
	this->owner = owner;

	pos = {-100, -100};
	moveSpeed = 500.0f;
	moveTime = 10.0f;
	size = 50;
	shape = { 0, 0, 0, 0 };
	damage = 5000;
	angle = 0.0f;
	isFired = false;
	missileType = TYPE::Normal;
	fireStep = 0;
	target = nullptr;
	destAngle = 0.0f;

	// 이미지
	img = ImageManager::GetSingleton()->AddImage("Bullet", "Image/Katana/enemy/enemy_bullet.bmp", 48, 2);

    return S_OK;
}

void Missile::Release()
{

}

void Missile::Update()
{
	// 위치 이동
	if (isFired)
	{
		switch (missileType)
		{
		case TYPE::Normal:
			MovingNormal();
			break;;
		}

		if (pos.x < 0 || pos.y < 0 || pos.x > WINSIZE_X || pos.y > WINSIZE_Y)
		{
			isFired = false;
			fireStep = 0;
		}
	}

	shape.left = pos.x - size / 2;
	shape.top = 0;
	shape.right = pos.x + size / 2;
	shape.bottom =2;
}

void Missile::Render(HDC hdc)
{
	if (isFired)
	{
		img->Render(hdc, pos.x, pos.y, true);
		RenderRectToCenter(hdc, pos.x, pos.y,size,2);
	}
}

void Missile::MovingNormal()
{
	float elapsedTime = TimerManager::GetSingleton()->GetElapsedTime();
	pos.x += cosf(angle) * moveSpeed * elapsedTime / moveTime;
	pos.y -= sinf(angle) * moveSpeed * elapsedTime / moveTime;
}

void Missile::MovingFollowTarget()
{
	if (target)
	{
	//	destAngle = GetAngle(pos, target->GetPos());
		float ratio = (destAngle - angle) / 50.0f;

		if (-0.01f < ratio && ratio < 0.01f)
		{
			angle = destAngle;
		}
		else
		{
			angle += ratio;
		}
	}

	pos.x += cosf(angle) * moveSpeed;
	pos.y -= sinf(angle) * moveSpeed;
}

void Missile::SetIsFired(bool isFired)
{
	this->isFired = isFired;
}
