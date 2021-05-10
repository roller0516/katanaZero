#pragma once
#include "GameNode.h"


class Player;
class Camera :
    public GameNode
{
private:
	FPOINT pos;
	int sizeX;
	int sizeY;
	int MapSizeX;
	int MapSizeY;
	Player* target;
	float moveSpeed;
public:
	HRESULT Init(Player* target);
	void Release();
	void Update();
	void Render(HDC hdc, Image* image);

};

