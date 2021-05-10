#include "Camera.h"
#include "Player.h"
#include "Image.h"
HRESULT Camera::Init(Player* target)
{
	this->target = target;

	sizeX = WINSIZE_X;
	sizeY = WINSIZE_Y;
	moveSpeed = 300.0f;
	return S_OK;
}

void Camera::Release()
{
}

void Camera::Update()
{
	if (KeyManager::GetSingleton()->IsStayKeyDown('D')) // ��
	{
		pos.x += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	}

	else if (KeyManager::GetSingleton()->IsStayKeyDown('S')) // �Ʒ�
	{
		pos.y += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown('A')) // ��
	{
		pos.x -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	}
}

void Camera::Render(HDC hdc,Image* image)
{
	
}


