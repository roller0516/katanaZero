#pragma once
#include "GameNode.h"
class Image;
class Player;
class CollisionManager :
    public GameNode
{
public :
    void pixelCollision(Player* player , COLORREF RGB, Image* bg);
};

