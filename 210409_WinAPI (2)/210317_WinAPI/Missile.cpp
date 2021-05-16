#include "Missile.h"
#include "Enemy.h"
#include "CommonFunction.h"
#include "Image.h"

HRESULT Missile::Init()
{
	pos = {-100, -100};
	moveSpeed = 5.0f;
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
	img = new Image();
	img->Init("Image/구슬.bmp", size, size, true, RGB(255, 0, 255));

    return S_OK;
}

void Missile::Release()
{
	//delete
	if (img)
	{
		img->Release();
		delete img;
		img = nullptr;
	}
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
			break;
		case TYPE::Skill_01:
			MovingSkill_01();
			break;
		case TYPE::FollowTarget:
			MovingFollowTarget();
			break;
		case TYPE::StarMoving:
			MovigStar();
		}
		if (pos.x < 0 || pos.y < 0 || pos.x > WINSIZE_X || pos.y > WINSIZE_Y)
		{
			
			isDraw = false;
		}
	}
	
	shape.left = pos.x - size / 2;
	shape.top = pos.y - size / 2;
	shape.right = pos.x + size / 2;
	shape.bottom = pos.y + size / 2;
}

void Missile::Render(HDC hdc)
{
	if(isDraw)
		img->Render(hdc, pos.x - (size / 2), pos.y - (size / 2));
		//Ellipse(hdc, shape.left, shape.top, shape.right, shape.bottom);

}

void Missile::MovingNormal()
{
	pos.x += cosf(angle) * moveSpeed;
	pos.y -= sinf(angle) * moveSpeed;
}

void Missile::MovingSkill_01()
{
	if (fireStep == 0 && pos.y < WINSIZE_Y)
	{
		angle = fireIndex * 3.14f * 2.0f / 36.0f;
		fireStep++;
	}

	pos.x += cosf(angle) * moveSpeed;
	pos.y -= sinf(angle) * moveSpeed;
}

void Missile::MovingFollowTarget()
{
	if (target)
	{
		destAngle = GetAngle(pos, target->GetPos());
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

void Missile::MovigStar()
{
	pos.x -= cosf(angle) * moveSpeed;
	pos.y += sinf(angle) * moveSpeed;
}

