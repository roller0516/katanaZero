#include "Enemy_Grunt.h"

HRESULT Enemy_Grunt::Init(int posX, int posY)
{
	this->type = EnemyType::Grunt;
	return S_OK;
}

void Enemy_Grunt::Release()
{
}

void Enemy_Grunt::Update()
{
}

void Enemy_Grunt::Render(HDC hdc)
{
}

Enemy* Enemy_Grunt::Clone()
{
	return new Enemy_Grunt;
}
