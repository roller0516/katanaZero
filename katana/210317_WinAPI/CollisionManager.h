#pragma once
#include "GameNode.h"
class InstallObject;
class ItemManager;
class MissileManager;
class EnemyManager;
class Image;
class Player;
class Enemy;
class BattleScene;
class CollisionManager :
    public GameNode
{
private:
    int count = 0;
public :
    bool MissilePlayerEnemy(MissileManager* missile , Player* player, EnemyManager* enemy, BattleScene* battleScene, int index);
    void EnemyPlayer(EnemyManager* Enemy, Player* player, BattleScene* battleScene,int index);
    void EnemyItem(Player* player,EnemyManager* Enemy, BattleScene* battleScene, ItemManager* item, int index);
    void PlayerDoorEnemy(Player* player, InstallObject* installobj, BattleScene* battleScene, EnemyManager* enemy,int index);
    void BossPlayer(Player* player, Enemy* boss);
    //void OilFlamebarrel(ItemManager* item);
    //void PlayerBoom(Player* player);
};

