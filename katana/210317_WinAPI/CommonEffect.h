#pragma once
#include "GameNode.h"

class Image;
class CommonEffect :
    public GameNode
{
private:
	FPOINT worldPos;
	FPOINT localPos;
	float currframe;
	float maxFrame;
	Image* flame;
	bool isAlive;
public:
	HRESULT Init(int x, int y);
	void Release();
	void Update();
	void Render(HDC hdc);
	bool GetAlive() { return this->isAlive; }
	void SetAlive(bool alive) { this->isAlive = alive; }
	void SetPos(int x, int y) { this->worldPos.x = x; this->worldPos.y = y; }
};

