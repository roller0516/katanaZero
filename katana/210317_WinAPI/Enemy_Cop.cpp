#include "Enemy_Cop.h"

HRESULT Enemy_Cop::Init(int posX, int posY)
{
	this->type = EnemyType::Cop;
	return S_OK;
}

void Enemy_Cop::Release()
{
}

void Enemy_Cop::Update()
{
}

void Enemy_Cop::Render(HDC hdc)
{
}

Enemy* Enemy_Cop::Clone()
{
	return new Enemy_Cop;
}
