#include "UI.h"
#include "Image.h"
HRESULT UI::Init()
{
	gui = ImageManager::GetSingleton()->AddImage("Gui", "Image/Katana/hud_collapse.bmp", 1280, 46, true, RGB(255, 0, 255));
	slowBar = ImageManager::GetSingleton()->AddImage("slowbar", "Image/Katana/effect/effect_battery.bmp", 110, 20, true, RGB(255, 0, 255));
	timerBar = ImageManager::GetSingleton()->AddImage("timerbar", "Image/Katana/effect/effect_timer.bmp", 188, 22, true, RGB(255, 0, 255));
	hud_go_arrow = ImageManager::GetSingleton()->AddImage("hud_go_arrow", "Image/Katana/effect/hud_go_arrow.bmp", 62, 38, true, RGB(255, 0, 255));
	hudGo = ImageManager::GetSingleton()->AddImage("hudGo", "Image/Katana/effect/hud_go.bmp", 78, 40, true, RGB(255, 0, 255));
	itmeIcon = ImageManager::GetSingleton()->AddImage("effect_black", "Image/Katana/item/effect_black.bmp", 1280, 720, 1, 1, true, RGB(255, 255, 255));
	guiPos.x = 0;
	guiPos.y = 0;
	timerBarPos.x = WINSIZE_X / 2 -88;
	timerBarPos.y = 5;
	hud_go_arrowPos.x = WINSIZE_X - 100;
	hud_go_arrowPos.y = 90;
	hudGoPos.x = WINSIZE_X - 100;
	hudGoPos.y = 150;
	slowBarPos.x = 23;
	slowBarPos.y = 12;
	itemIconPos.x = WINSIZE_X - 60;
	itemIconPos.y = 10;
	return S_OK;
	
}

void UI::Release()
{

}

void UI::Update()
{
	timerCoolTime += TimerManager::GetSingleton()->GetElapsedTime();
	if (timerCoolTime > 1) 
	{
		timerCoolTime = 0;
		timerBar->GetImageInfo()->width -= 0.001;
	}

	hud_go_arrowPos.x += 50 * TimerManager::GetSingleton()->GetElapsedTime();
	if(hud_go_arrowPos.x > WINSIZE_X - 70)
		hud_go_arrowPos.x = WINSIZE_X - 100;
}

void UI::Render(HDC hdc)
{
	gui->Render(hdc, guiPos.x, guiPos.y);
	slowBar->Render(hdc, slowBarPos.x, slowBarPos.y);
	timerBar->Render(hdc, timerBarPos.x, timerBarPos.y);
	if(itmeIcon)
		itmeIcon->Render(hdc, itemIconPos.x, itemIconPos.y);
	if (clear) 
	{
		hud_go_arrow->Render(hdc, hud_go_arrowPos.x, hud_go_arrowPos.y, true);
		hudGo->Render(hdc, hudGoPos.x, hudGoPos.y, true);
	}
}
	
void UI::SetItemImage(string name)
{
	itmeIcon = ImageManager::GetSingleton()->FindImage(name);
}
