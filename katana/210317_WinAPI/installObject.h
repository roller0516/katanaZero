#pragma once
#include "GameNode.h"

class Image;
class InstallObject :
    public GameNode
{
private:
	Image* img;
	FPOINT worldPos;
	FPOINT localPos;

	InstallType Type;
	RECT shape;
	RECT attackShape;

	int size; 

	float currframe;
	float maxframe;

	bool world;
	bool oPen;
	bool close;

	vector <InstallObject*> vObject;
public:
	HRESULT Init(int x, int y, InstallType Type);
	void Release();
	void Update();
	void Render(HDC hdc);

	void SetWrold(bool world) { this->world = world; }
	void SetOpen(bool open) { this->oPen = open; }
	bool GetClose() { return close; }
	
	RECT GetShape() { return shape; }
	RECT GetAttackShape() { return attackShape; }

	void AddObject();

	InstallObject* Clone();
};

