#include "CollisionManager.h"
#include "Player.h"
#include "Image.h"
#include "Camera.h"
void CollisionManager::pixelCollision(Player* player, COLORREF RGB, Image* bg)
{
	COLORREF color = RGB;
	int R, G, B;
	float playerHeight = 35;
	float currPosY = player->GetWorldpos().y + playerHeight;
	for (int i = currPosY - 10; i < currPosY + 10; i++)
	{
		color = GetPixel(bg->GetMemDC(),
			player->GetWorldpos().x, i);

		R = GetRValue(color);
		G = GetGValue(color);
		B = GetBValue(color);

		if (!(R == 255 && G == 0 && B == 255))
		{
			player->SetPosY(i - playerHeight-1);
			player->SetGround(true);
			break;
		}
	}


}
