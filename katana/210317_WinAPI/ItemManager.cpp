#include "ItemManager.h"
#include "Camera.h"
#include "Image.h"
#include "Player.h"
#include "CommonFunction.h"
HRESULT Item::Init(string name,int x,int y)
{
	ImageManager::GetSingleton()->AddImage("Knife", "Image/Katana/item/item_knife.bmp",34,34,1,1,true,RGB(255,0,255));
	ImageManager::GetSingleton()->AddImage("Oil", "Image/Katana/item/item_molotov.bmp", 18, 40, 1, 1 ,true, RGB(255, 0, 255));
	//ImageManager::GetSingleton()->AddImage("Smoke", "Image/Katana/item/item_beer_get.bmp", 12, 30, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("bottle", "Image/Katana/item/item_bottle.bmp", 12, 30, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Bust", "Image/Katana/item/item_bust.bmp", 38, 56, 1, 1, true, RGB(255, 0, 255));

	ImageManager::GetSingleton()->AddImage("Knife_get", "Image/Katana/item/item_knife_get.bmp", 42,42, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Oil_get", "Image/Katana/item/item_molotov_get.bmp", 26, 48, 1, 1, true, RGB(255, 0, 255));
	//ImageManager::GetSingleton()->AddImage("Smoke", "Image/Katana/item/item_beer_get.bmp", 12, 30, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("bottle_get", "Image/Katana/item/item_beer_get.bmp", 20, 38, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Bust_get", "Image/Katana/item/item_bust_get.bmp", 46, 64, 1, 1, true, RGB(255, 0, 255));
	imgArrow = ImageManager::GetSingleton()->AddImage("item_Arrow", "Image/Katana/item/item_pickuparrow_8x1.bmp", 272, 42, 8, 1, true, RGB(255, 0, 255));

	img = ImageManager::GetSingleton()->FindImage(name);
	
	/*itemType = ItemType::None;*/
	this->name = name;
	worldPos.x = x;
	worldPos.y = y;
	localPos = { 0,0 };
	angle = 0;
	isFire = false;
	getItem = false;
	isAlive = true;
	isAlpha = false;
	hitShape = { 0,0,0,0 };
	moveSpeed = 700;
	alpha = 255;
	size = img->GetImageInfo()->frameWidth;
	return S_OK;
}

void Item::Release()
{

}

void Item::Update()
{
	if (isFire) 
	{
		ChangeImage(false);
		alpha = 0;
		arrowOn = false;
		getItem = false;
		isAlpha = false;
		fireAngle += 15*TimerManager::GetSingleton()->GetElapsedTime();
		Fire();
	}

	if (isAlive == false)
		hitShape = { -100,-100,-100,-100 };

	if (arrowOn) 
	{
		currFrame += 15 * TimerManager::GetSingleton()->GetElapsedTime();
		if (currFrame > 8)
			currFrame = 0;
	}

	
	
	if (getItem) 
	{
		alphaTime += 15 * TimerManager::GetSingleton()->GetElapsedTime();
		if (count < 3)
		{
			if (alphaTime > 2 && alphaTime < 4)
			{
				alpha = 255;

			}
			else if (alphaTime > 4)
			{
				alpha = 0;
				alphaTime = 0;
				count++;
			}
		}
		worldPos = owner->GetWorldpos();
	}

	localPos.x = worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
	localPos.y = worldPos.y - Camera::GetSingleton()->GetCameraPos().y;
	
	hitShape.left = localPos.x - size / 2;
	hitShape.top = localPos.y - size / 2;
	hitShape.right = localPos.x + size / 2;
	hitShape.bottom = localPos.y + size / 2;
}

void Item::Render(HDC hdc)
{
	if (isAlpha)
		img->AlphaRender(hdc, owner->Getpos().x, owner->Getpos().y - 80, 0, 0, alpha, true);
	if (isAlive) 
	{
		if (isFire && angle < DegToRad(90) && angle > DegToRad(-90))
			img->rotateRender(hdc, localPos.x, localPos.y, 0, 0, fireAngle);
		else if(isFire && angle >= DegToRad(90) || angle <= DegToRad(-90))
			img->rotateRenderFlip(hdc, localPos.x, localPos.y, 0, 0, fireAngle);
		else
			img->Render(hdc, localPos.x, localPos.y, true);
		if (arrowOn && isFire ==false)
			imgArrow->FrameRender(hdc, localPos.x, localPos.y - 50, currFrame, 0, true);
	}
}
void Item::Fire()
{
	float elapsedTime = TimerManager::GetSingleton()->GetElapsedTime();
	worldPos.x += cosf(angle) * moveSpeed * elapsedTime;
	worldPos.y -= sinf(angle) * moveSpeed * elapsedTime;
}
void Item::ChangeImage(bool change)
{
	if (change) 
	{
		img = ImageManager::GetSingleton()->FindImage(name + "_get");
		isAlpha = true;
	}
	else
		img = ImageManager::GetSingleton()->FindImage(name);
}
Item* Item::Clone()
{
	return new Item;
}
HRESULT ItemManager::Init(int x, int y,Player* owner)
{
	for (int i = vItem.size()-1; i < vItem.size(); i++)
	{
		vItem[i]->Init(name,x, y);
		vItem[i]->SetOwner(owner);
	}
	return S_OK;
}

void ItemManager::Release()
{

}

void ItemManager::Update()
{
	for (int i = 0; i < vItem.size(); i++) 
	{
		vItem[i]->Update();
	}
}

void ItemManager::Render(HDC hdc)
{
	for (int i = 0; i < vItem.size(); i++)
	{
		vItem[i]->Render(hdc);
	}
}

void ItemManager::AddItem(string name, Item* item)
{
	this->name = name;
	vItem.push_back(item->Clone());
}



