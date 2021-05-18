#include "Camera.h"
#include "Player.h"
#include "Image.h"
#include "TilemapTool.h"
HRESULT Camera::Init(Player* target)
{
	if (this->TilemapTooltarget)
		this->TilemapTooltarget = nullptr;

	this->target = target;

	bg = ImageManager::GetSingleton()->AddImage("stage1_bg_render", "Image/Katana/stage1_bg_render.bmp", 2176, 3500);
	bg_Collision = ImageManager::GetSingleton()->AddImage("stage1_bg_collision", "Image/Katana/stage1_bg_collision.bmp", 2176, 3500);
	worldrc.x = 2176;
	worldrc.y = 3500;
	pivot.x = 0.5f;
	pivot.y = 0.5f;
	return S_OK;
}
HRESULT Camera::Init(TilemapTool* target)
{
	if (this->target)
		this->target = nullptr;
	this->TilemapTooltarget = target;

	bg = ImageManager::GetSingleton()->AddImage("stage1_bg_render", "Image/Katana/stage1_bg_render.bmp", 2176, 3500);
	bg_Collision = ImageManager::GetSingleton()->AddImage("stage1_bg_collision", "Image/Katana/stage1_bg_collision.bmp", 2176, 3500);

	worldrc.x = 2176;
	worldrc.y = 3500;
	pivot.x = 0.5f;
	pivot.y = 0.5f;
	worldPos.x = WINSIZE_X/2;
	worldPos.y = WINSIZE_Y/2;
	pos.x = 0;
	pos.y = 0;
	return S_OK;
}

void Camera::Release()
{

}
void Camera::Update()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown('Z'))
		debug = true;
	if (KeyManager::GetSingleton()->IsOnceKeyDown('X'))
		debug = false;
}


void Camera::Render(HDC hdc)
{
	bg_Collision->CameraRender(hdc, pos.x, pos.y, WINSIZE_X, WINSIZE_Y,false);
	if(debug)
		bg->CameraRender(hdc, pos.x, pos.y, WINSIZE_X, WINSIZE_Y, false);
}

void Camera::View()
{
	FPOINT playerPos;
	if (target!=nullptr)
		playerPos = target->GetWorldpos();
	else if(TilemapTooltarget != nullptr)
		playerPos = TilemapTooltarget->GetTileMapPos();

	FPOINT tPos;

	float angle = GetAngle(playerPos, GetWorldMousePos(pos));
	float x = Distance(playerPos, GetWorldMousePos(pos)).x;
	float y = Distance(playerPos, GetWorldMousePos(pos)).y;

	float distance = sqrtf(powf(x, 2) + powf(y, 2));

	FPOINT dest;

	dest.x = cosf(angle) * distance * 0.09f;
	dest.y = sinf(angle) * distance * 0.05f;

	tPos.x = playerPos.x + dest.x;
	tPos.y = playerPos.y - dest.y;

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
FPOINT Camera::Distance(FPOINT pos, POINT pos2)
{
	FPOINT temp;

	temp.x = abs(pos.x - pos2.x);
	temp.y = abs(pos.y - pos2.y);

	return temp;
}
FPOINT Camera::Distance(FPOINT pos, FPOINT pos2)
{
	FPOINT temp;

	temp.x = abs(pos.x - pos2.x);
	temp.y = abs(pos.y - pos2.y);

	return temp;
}
float Camera::GetAngle(FPOINT pos, POINT targetPos)
{
	float angle = 0.0f;

	float x = targetPos.x - pos.x;
	float y = targetPos.y - pos.y;

	angle = atan2(-y, x);

	return angle;
}
float Camera::GetAngle(FPOINT pos, FPOINT targetPos)
{
	float angle = 0.0f;

	float x = targetPos.x - pos.x;
	float y = targetPos.y - pos.y;

	angle = atan2(-y, x);

	return angle;
}
FPOINT Camera::GetWorldMousePos(FPOINT pos)
{
	FPOINT ClientSize;
	FPOINT worldMousePos;

	ClientSize.x = WINSIZE_X / 2;
	ClientSize.y = WINSIZE_Y / 2;
	if (pos.x > WINSIZE_X / 2)
		worldMousePos.x = pos.x - ClientSize.x + g_ptMouse.x;
	else
		worldMousePos.x = g_ptMouse.x;
	if (pos.y > WINSIZE_Y / 2)
		worldMousePos.y = pos.y - ClientSize.y + g_ptMouse.y;
	else
		worldMousePos.y = g_ptMouse.y;

	return worldMousePos;

}





