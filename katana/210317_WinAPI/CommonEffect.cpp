#include "CommonEffect.h"
#include "Camera.h"
#include "Image.h"
HRESULT CommonEffect::Init(int x, int y)
{
	worldPos.x = x;
	worldPos.y = y;
	isAlive = false;
	flame = ImageManager::GetSingleton()->AddImage("FlameEffect", "Image/Katana/effect/effect_explosion_12x1.bmp", 2112, 224, 11, 1, true, RGB(255, 0, 255));
	maxFrame = 11;
	currframe = 0;
	return S_OK;
}

void CommonEffect::Release()
{

}

void CommonEffect::Update()
{
	currframe += 15 * TimerManager::GetSingleton()->GetElapsedTime();
	if (isAlive) 
	{
		if (currframe > maxFrame)
		{
			currframe = 0;
			isAlive = false;
		}
	}

	localPos.x = worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
	localPos.y = worldPos.y - Camera::GetSingleton()->GetCameraPos().y;
}

void CommonEffect::Render(HDC hdc)
{
	if(isAlive)
		flame->FrameRender(hdc, localPos.x, localPos.y, currframe, 0, true);
}
