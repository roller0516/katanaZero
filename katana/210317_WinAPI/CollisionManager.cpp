#include "CollisionManager.h"
#include "Player.h"
#include "Image.h"
#include "Camera.h"
#include "CommonFunction.h"


void CollisionManager::Render(HDC hdc, Player* player)
{
	RenderRectToCenter(hdc, player->Getpos().x, player->Getpos().y + 40,10,10);
}
