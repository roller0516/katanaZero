#pragma once
#include "GameNode.h"
class InstallObject;
class ItemManager;
class MissileManager;
class EnemyManager;
class Image;
class Player;
class Enemy;
class CollisionManager :
    public GameNode
{
private:
    int count = 0;
public :
    void MissilePlayerEnemy(MissileManager* missile , Player* player, EnemyManager* enemy,int index);
    void EnemyPlayer(EnemyManager* Enemy, Player* player,int index);
    void EnemyItem(Player* player,EnemyManager* Enemy, ItemManager* item, int index);
    void PlayerDoorEnemy(Player* player, InstallObject* installobj, EnemyManager* enemy,int index);
    void BossPlayer(Player* player, Enemy* boss);
    //void OilFlamebarrel(ItemManager* item);
    //void PlayerBoom(Player* player);
};

