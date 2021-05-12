#include "Camera.h"
#include "Player.h"
#include "Image.h"
#include "CommonFunction.h"
HRESULT Camera::Init(Player* target)
{
	this->target = target;

	bg = ImageManager::GetSingleton()->AddImage("stage1_bg_render", "Image/Katana/stage1_bg_render.bmp", 2176, 3500);
	bg_Collision = ImageManager::GetSingleton()->AddImage("stage1_bg_collision", "Image/Katana/stage1_bg_collision.bmp", 2716, 3500);
	worldrc.x = 2176;
	worldrc.y = 3500;
	pivot.x = 0.5f;
	pivot.y = 0.5f;
	pos.x = 0; 
	pos.y = 0;
	startPos = pos;
	return S_OK;
}

void Camera::Release()
{
	
}



void Camera::Update()
{
	//Lerp : startPos * (1 - alpha) + endPos * alpha
	//P = (1-t)P1 + tP2
	
	if (target) 
	{

		FPOINT playerPos = target->GetWorldpos();
		FPOINT tPos;
		
		float angle = GetAngle(target->GetWorldpos(), GetWorldMousePos(pos));
		float x = Distacne(target->GetWorldpos(), GetWorldMousePos(pos)).x;
		float y = Distacne(target->GetWorldpos(), GetWorldMousePos(pos)).y;

		float distance = sqrtf(powf(x, 2) + powf(y, 2));

		FPOINT dest;

		dest.x = cosf(angle)* distance * 0.09f;
		dest.y = sinf(angle)* distance * 0.05f;
		
		tPos.x = playerPos.x + dest.x;
		tPos.y = playerPos.y - dest.y;
		

		RECT tSize = target->GetRect();

		float leftArea = WINSIZE_X * pivot.x;
		float rightArea = WINSIZE_X - leftArea;
		float topArea = WINSIZE_Y * pivot.y;
		float bottomArea = WINSIZE_Y - topArea;

		if (tPos.x <= leftArea)
			pos.x = 0.f;
		else if (tPos.x >= worldrc.x - rightArea)
			pos.x = worldrc.x - WINSIZE_X;
		else
			pos.x = tPos.x - WINSIZE_X * pivot.x;

		if (tPos.y <= topArea)
			pos.y = 0.f;
		else if (tPos.y >= worldrc.y - bottomArea)
			pos.y = worldrc.y - WINSIZE_Y;
		else
			pos.y = tPos.y - WINSIZE_Y * pivot.y;
	}
}


void Camera::Render(HDC hdc)
{
	bg->CameraRender(hdc, pos.x, pos.y, WINSIZE_X, WINSIZE_Y, false);
	bg_Collision->CameraRender(hdc, pos.x, pos.y, WINSIZE_X, WINSIZE_Y, false);
}





