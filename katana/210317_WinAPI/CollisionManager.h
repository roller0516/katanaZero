#pragma once
#include "GameNode.h"
class Image;
class Player;
class CollisionManager :
    public GameNode
{
public :
    void pixelCollision(Player* player, Image* bg);
    void Render(HDC hdc,Player* player);
};

