#include "CollisionManager.h"
#include "Player.h"
#include "Image.h"
#include "Camera.h"
#include "CommonFunction.h"
#include "MissileManager.h"
#include "ItemManager.h"
#include "Missile.h"
#include "EnemyManager.h"
#include "Enemy.h"

void CollisionManager::MissilePlayerEnemy(MissileManager* missile, Player* player, EnemyManager* enemy, int index)
{
	RECT rcTemp, rcplayer, rcmissile,rcReflect,rcEnemy;
	rcplayer = player->GetRect();
	rcReflect = player->GetAttackShape();
	rcEnemy = enemy->GetMonsterList()[index]->GetData()->shape;
	for (int i = 0; i < missile->GetMissile().size(); i++) 
	{
		rcmissile = missile->GetMissile()[i]->GetRect();
		if (missile->GetMissile()[i]->GetIsFired() == true 
			&& missile->GetMissile()[i]->GetType() == MissileType::enemy 
			&& IntersectRect(&rcTemp,&rcplayer,&rcmissile))
		{
			if (IntersectRect(&rcTemp, &rcReflect, &rcmissile))
			{
				missile->GetMissile()[i]->SetAngle(player->GetPlayerAngle());
				missile->GetMissile()[i]->SetType(MissileType::player);
				break;
			}
			missile->GetMissile()[i]->SetPos(-100, -100);
			missile->GetMissile()[i]->SetLocalPos(-100,-100);
			missile->GetMissile()[i]->SetIsFired(false);
			break;
		}

		if (missile->GetMissile()[i]->GetIsFired() == true
			&& missile->GetMissile()[i]->GetType() == MissileType::player
			&& IntersectRect(&rcTemp, &rcEnemy, &rcmissile))
		{
			missile->GetMissile()[i]->SetPos(-100, -100);
			missile->GetMissile()[i]->SetLocalPos(-100, -100);
			missile->GetMissile()[i]->SetIsFired(false);
			enemy->GetMonsterList()[index]->GetData()->isAlive = false;
			enemy->GetMonsterList()[index]->GetData()->shape = { -100,-100,-100,-100 };
			break;
		}
	}
}


//void CollisionManager::PlayerDoor(Player* player, EnemyManager* enemy)
//{
//
//}

void CollisionManager::EnemyPlayer(EnemyManager* enemy, Player* player,int index)
{
	RECT rcTemp, rcplayer, rcEnemy;
	rcplayer = player->GetAttackShape();
	rcEnemy = enemy->GetMonsterList()[index]->GetData()->shape;
	if (IntersectRect(&rcTemp, &rcplayer, &rcEnemy))
	{
		enemy->GetMonsterList()[index]->GetData()->isAlive = false;
		enemy->GetMonsterList()[index]->GetData()->shape = { -100,-100,-100,-100 };
	}
}

void CollisionManager::EnemyItem(Player* player,EnemyManager* enemy, ItemManager* item,int index)
{
	RECT rcTemp, rcitem, rcEnemy;
	rcitem = item->GetItemList()[player->GetItemIndex()]->GetRect();
	rcEnemy = enemy->GetMonsterList()[index]->GetData()->shape;
	if (IntersectRect(&rcTemp, &rcitem, &rcEnemy)&& item->GetItemList()[player->GetItemIndex()]->Getfired()==true)
	{
		enemy->GetMonsterList()[index]->GetData()->isAlive = false;
		enemy->GetMonsterList()[index]->GetData()->shape = {-100,-100,-100,-100};
		item->GetItemList()[player->GetItemIndex()]->SetAlive(false);
	}
}

