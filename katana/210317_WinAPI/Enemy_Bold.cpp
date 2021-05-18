#include "Enemy_Bold.h"

HRESULT Enemy_Bold::Init(int posX, int posY)
{
	return S_OK;
}

void Enemy_Bold::Release()
{
}

void Enemy_Bold::Update()
{
}

void Enemy_Bold::Render(HDC hdc)
{
}

Enemy* Enemy_Bold::Clone()
{
	return new Enemy_Bold;
}
