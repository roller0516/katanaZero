#include "Missile.h"
#include "Enemy.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Camera.h"

HRESULT Missile::Init()//Enemy* owner)
{
	//this->owner = owner;

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
	ImageManager::GetSingleton()->AddImage("Top_lazer", "Image/Katana/boss/boss_lazer_top_10x1.bmp", 360, 1500, 10, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Ground_lazer", "Image/Katana/boss/boss_lazer_ground_1x10.bmp", 1500, 360, 1, 10, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Mine", "Image/Katana/boss/boss_mine_1x2.bmp", 40, 20, 2, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Circle", "Image/Katana/boss/boss_circle.bmp", 274, 274, 1, 1, true, RGB(255, 0, 255));
    return S_OK;
}

void Missile::Release()
{

}

void Missile::Update()
{
	
	if (isFired)
	{
		delay += TimerManager::GetSingleton()->GetElapsedTime();
		if (delay > coolTime) 
		{
			switch (missileType)
			{
			case MissileType::Toplazer:
				Toplazer();
				break;
			case MissileType::lazer:
				lazer();
				break;
			case MissileType::Normal:
				MovingNormal();
				break;
			case MissileType::Mine:
				Mine();
				break;
			}
			if (missileType != MissileType::Normal) 
			{
				currFrame += 15 * TimerManager::GetSingleton()->GetElapsedTime();
				if (currFrame > maxFrame)
				{
					currFrame = 0;
					isFired = false;
					delay = 0;
				}
			}
		}
	}
}

void Missile::Render(HDC hdc)
{
	if (isFired)
	{
		localPos.x = worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
		localPos.y = worldPos.y - Camera::GetSingleton()->GetCameraPos().y;

		switch (missileType)
		{
		case MissileType::Toplazer:
				img->FrameRender(hdc, localPos.x, localPos.y, currFrame, 0, true);
			break;
		case MissileType::lazer:
			if(angle == 0)
				img->FrameRenderFlip(hdc, localPos.x, localPos.y, 0, currFrame, true);
			else
				img->FrameRender(hdc, localPos.x, localPos.y, 0, currFrame, true);
			break;
		case MissileType::Normal:
			if (angle >= 0 && angle <DegToRad(10) || angle > DegToRad(170) && angle <= DegToRad(180))
				img->FrameRender(hdc, localPos.x, localPos.y, 0, 0, true, 1.3f);
			else
				img->rotateRender(hdc, localPos.x, localPos.y, 0, 0, angle);
			break;
		case MissileType::Mine:
			break;
		}
		//Rectangle(hdc, shape.left - Camera::GetSingleton()->GetCameraPos().x , shape.top - Camera::GetSingleton()->GetCameraPos().y ,
		//	shape.right - Camera::GetSingleton()->GetCameraPos().x , shape.bottom- Camera::GetSingleton()->GetCameraPos().y);
	}
}

void Missile::MovingNormal()
{
	float elapsedTime = TimerManager::GetSingleton()->GetElapsedTime();
	img = ImageManager::GetSingleton()->FindImage("Bullet");
	worldPos.x += cosf(angle) * moveSpeed * elapsedTime;
	worldPos.y -= sinf(angle) * moveSpeed * elapsedTime;

	shape.left = worldPos.x - size / 2;
	shape.top = worldPos.y - 1;
	shape.right = worldPos.x + size / 2;
	shape.bottom = worldPos.y + 1;
}

void Missile::lazer()
{
	maxFrame = 10;
	img = ImageManager::GetSingleton()->FindImage("Ground_lazer");
	shape.left = worldPos.x - img->GetImageInfo()->frameWidth / 2;
	shape.top = worldPos.y - img->GetImageInfo()->frameHeight / 2;
	shape.right = worldPos.x + img->GetImageInfo()->frameWidth / 2;
	shape.bottom = worldPos.y + img->GetImageInfo()->frameHeight / 2;
}

void Missile::Toplazer()
{
	maxFrame = 10;
	img = ImageManager::GetSingleton()->FindImage("Top_lazer");
	shape.left = worldPos.x - img->GetImageInfo()->frameWidth / 2;
	shape.top = worldPos.y - img->GetImageInfo()->frameHeight / 2;
	shape.right = worldPos.x + img->GetImageInfo()->frameWidth / 2;
	shape.bottom = worldPos.y + img->GetImageInfo()->frameHeight / 2;
}

void Missile::Mine()
{

}

void Missile::SetIsFired(bool isFired)
{
	this->isFired = isFired;
}
