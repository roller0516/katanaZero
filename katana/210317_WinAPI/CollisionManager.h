#pragma once
#include "GameNode.h"
class InstallObject;
class ItemManager;
class MissileManager;
class EnemyManager;
class Image;
class Player;
class CollisionManager :
    public GameNode
{
public :
    void MissilePlayerEnemy(MissileManager* missile , Player* player, EnemyManager* enemy,int index);
    void EnemyPlayer(EnemyManager* Enemy, Player* player,int index);
    void EnemyItem(Player* player,EnemyManager* Enemy, ItemManager* item, int index);
    void PlayerDoor(Player* player, InstallObject* installobj);
    //void OilFlamebarrel(ItemManager* item);
    //void PlayerGetItem(Player* player, ItemManager* item, int index);
    //void PlayerBoom(Player* player);
    //void EnemyItem(EnemyManager* enemy,Item* item);
};

