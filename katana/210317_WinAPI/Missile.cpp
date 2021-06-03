#include "Missile.h"
#include "Enemy.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Camera.h"
#include "CommonEffect.h"

HRESULT Missile::Init()//Enemy* owner)
{
	//this->owner = owner;

	worldPos = {-100, -100};
	shape = { 0, 0, 0, 0 };
	moveSpeed = 500;
	moveTime = 10.0f;
	size = 50;
	angle = 0.0f;
	isFired = false;
	fireStep = 0;
	destAngle = 0.0f;
	velocity = 80.0f;

	flame = new CommonEffect[8];

	missileType = MissileType::Normal;
	for(int i = 0 ; i < 8; i++)
	{
		flame[i].Init(-100, -100);
	}

	// ÀÌ¹ÌÁö
	img = ImageManager::GetSingleton()->AddImage("Bullet", "Image/Katana/enemy/enemy_bullet.bmp", 48, 2,1,1,true, RGB(255,0,255));
	ImageManager::GetSingleton()->AddImage("Top_lazer", "Image/Katana/boss/boss_lazer_top_10x1.bmp", 360, 1500, 10, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Ground_lazer", "Image/Katana/boss/boss_lazer_ground_1x10.bmp", 1500, 360, 1, 10, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Mine", "Image/Katana/boss/boss_mine_1x2.bmp", 40, 20, 2, 1, true, RGB(255, 0, 255));
	
	minerange = ImageManager::GetSingleton()->AddImage("Circle", "Image/Katana/boss/boss_circle.bmp", 274, 274, 1, 1, true, RGB(255, 0, 255));
	
	
    return S_OK;
}

void Missile::Release()
{

}

void Missile::Update()
{
	
	for (int i = 0; i < 6; i++)
	{
		flame[i].Update();
	}

	if (isFired)
	{
		localPos.x = worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
		localPos.y = worldPos.y - Camera::GetSingleton()->GetCameraPos().y;

		delay += TimerManager::GetSingleton()->GetElapsedTime();
		if (missileType == MissileType::Mine)
		{
			currFrame += 15 * TimerManager::GetSingleton()->GetElapsedTime();
			if (currFrame > maxFrame)
			{
				currFrame = 0;
				if (flameOn)
					flameOn = false;
			}

			if (fireIndex == 0)
			{
				fireIndex++;
				fallForce = 300;
				moveSpeed = 1000;
			}
			Mine();
			
			if (delay >= coolTime + 1.5f)
			{
				flameOn = false;
				isFired = false;
				fireIndex = 0;
				delay = 0;
			}
			else if (delay >= coolTime + 1 && delay < coolTime + 1.5f)
			{
				flamePos = localPos;
				flameOn = true;
				shape.left = worldPos.x - 274 / 2;
				shape.top = worldPos.y - 274 / 2;
				shape.right = worldPos.x + 274 / 2;
				shape.bottom = worldPos.y + 274 / 2;
				fallForce = 0;
			}
			else if (delay > coolTime && delay < coolTime + 1)
			{
				moveSpeed = 0;
				fallForce -= Gravity * TimerManager::GetSingleton()->GetElapsedTime() * velocity;
				worldPos.y -= fallForce * TimerManager::GetSingleton()->GetElapsedTime();
			}
		}


		else if (delay > coolTime)
		{
			moveSpeed = 500;
			switch (missileType)
			{
			case MissileType::Normal:
				MovingNormal();
				break;
			case MissileType::Toplazer:
				Toplazer();
				break;
			case MissileType::lazer:
				lazer();
				break;
				
			}

			if (missileType == MissileType::Toplazer || missileType == MissileType::lazer)
			{
				currFrame += 15 * TimerManager::GetSingleton()->GetElapsedTime();
				if (currFrame > maxFrame)
				{
					currFrame = 0;
					isFired = false;
					delay = 0;
				}
			}
			else if (localPos.x < 0 || localPos.y < 0 || localPos.x > WINSIZE_X || localPos.y > WINSIZE_Y)
			{
				isFired = false;
				delay = 0;
			}
		}
	}
	else 
	{
		shape = { -100,-100,-100,-100 };
		worldPos = { -100,-100 };
		localPos = { -100,-100 };
	}
		

	if (flameOn)
	{
		for (int i = 0; i <8; i++) 
		{
			if (flame[i].GetAlive() == false) 
			{
				flame[i].SetAlive(true);
			}
		}

		flame[0].SetPos(flamePos.x, flamePos.y - 70);
		flame[1].SetPos(flamePos.x, flamePos.y + 70);
		flame[2].SetPos(flamePos.x + 70, flamePos.y);
		flame[3].SetPos(flamePos.x - 70, flamePos.y);
		flame[4].SetPos(flamePos.x + 70, flamePos.y + 70);
		flame[5].SetPos(flamePos.x - 70, flamePos.y - 70);
		flame[6].SetPos(flamePos.x + 70, flamePos.y - 70);
		flame[7].SetPos(flamePos.x - 70, flamePos.y + 70);
	}
}


void Missile::Render(HDC hdc)
{
	if (isFired)
	{
		switch (missileType)
		{
		case MissileType::Normal:
			if (angle >= 0 && angle <DegToRad(10) || angle > DegToRad(170) && angle <= DegToRad(180))
				img->FrameRender(hdc, localPos.x, localPos.y, 0, 0, true, 1.3f);
			else
				img->rotateRender(hdc, localPos.x, localPos.y, 0, 0, angle);
			break;
		case MissileType::Toplazer:
				img->FrameRender(hdc, localPos.x, localPos.y, currFrame, 0, true);
			break;
		case MissileType::lazer:
			if(angle == 0)
				img->FrameRenderFlip(hdc, localPos.x, localPos.y, 0, currFrame, true);
			else
				img->FrameRender(hdc, localPos.x, localPos.y, 0, currFrame, true);
			break;
		case MissileType::Mine:
			img->FrameRender(hdc, localPos.x, localPos.y, currFrame, 0,true);
			minerange->FrameRender(hdc, localPos.x, localPos.y, 0, 0, true,1);
			circleSize = 2;
			break;
		}

		Rectangle(hdc, shape.left - Camera::GetSingleton()->GetCameraPos().x, shape.top - Camera::GetSingleton()->GetCameraPos().y,
			shape.right - Camera::GetSingleton()->GetCameraPos().x, shape.bottom - Camera::GetSingleton()->GetCameraPos().y);

		for (int i = 0; i < 8; i++) 
		{
			flame[i].Render(hdc);
		}
	}
}	

void Missile::MovingNormal()
{
	img = ImageManager::GetSingleton()->FindImage("Bullet");

	worldPos.x += cosf(angle) * moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();;
	worldPos.y -= sinf(angle) * moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();;

	shape.left = worldPos.x - size /4;
	shape.top = worldPos.y - size / 4;
	shape.right = worldPos.x + size / 4;
	shape.bottom = worldPos.y + size / 4;
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
	maxFrame = 2;
	img = ImageManager::GetSingleton()->FindImage("Mine");
	
	worldPos.x += cosf(angle) * moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	worldPos.y -= sinf(angle) * moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();

	shape.left = worldPos.x - img->GetImageInfo()->frameWidth / 2;
	shape.top = worldPos.y - img->GetImageInfo()->frameHeight / 2;
	shape.right = worldPos.x + img->GetImageInfo()->frameWidth / 2;
	shape.bottom = worldPos.y + img->GetImageInfo()->frameHeight / 2;
}
