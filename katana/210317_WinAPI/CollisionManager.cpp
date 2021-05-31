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
#include "installObject.h"

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
			&& IntersectRect(&rcTemp, &rcEnemy, &rcmissile)) //�ݻ�
		{
			TimerManager::GetSingleton()->SetTimeStop(true);
			player->ReflectEffect();
			player->HitEffect(enemy->GetMonsterList()[index]->GetData()->worldPos.x,
				enemy->GetMonsterList()[index]->GetData()->worldPos.y);
			missile->GetMissile()[i]->SetPos(-100, -100);
			missile->GetMissile()[i]->SetLocalPos(-100, -100);
			missile->GetMissile()[i]->SetIsFired(false);
			enemy->GetMonsterList()[index]->GetData()->isAlive = false;
			enemy->GetMonsterList()[index]->GetData()->shape = { -100,-100,-100,-100 };
			break;
		}
	}
}


void CollisionManager::EnemyPlayer(EnemyManager* enemy, Player* player,int index)
{
	RECT rcTemp, rcplayer, rcEnemy;
	rcplayer = player->GetAttackShape();
	rcEnemy = enemy->GetMonsterList()[index]->GetData()->shape;
	if (IntersectRect(&rcTemp, &rcplayer, &rcEnemy))
	{
		player->HitEffect(enemy->GetMonsterList()[index]->GetData()->worldPos.x , 
			enemy->GetMonsterList()[index]->GetData()->worldPos.y);
		TimerManager::GetSingleton()->SetTimeStop(true);
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
		player->HitEffect(enemy->GetMonsterList()[index]->GetData()->worldPos.x,
			enemy->GetMonsterList()[index]->GetData()->worldPos.y);
		TimerManager::GetSingleton()->SetTimeStop(true);
		enemy->GetMonsterList()[index]->GetData()->isAlive = false;
		item->GetItemList()[player->GetItemIndex()]->SetAlive(false);
		//item->GetItemList()[player->GetItemIndex()]->SetRect();
	}
}

void CollisionManager::PlayerDoorEnemy(Player* player, InstallObject* installobj, EnemyManager* enemy, int index)
{
	RECT rcTemp, rcTemp2 ,rcPlayer, rcDoor, rcEnemy, rcDoorAttack;
	rcPlayer = player->GetRect();
	rcDoor = installobj->GetShape();
	rcDoorAttack = installobj->GetAttackShape();
	rcEnemy = enemy->GetMonsterList()[index]->GetData()->shape;
	if (IntersectRect(&rcTemp, &rcPlayer, &rcDoor) && installobj->GetClose())
	{
		player->SetPos(player->GetWorldpos().x, player->GetWorldpos().y);
		if (KeyManager::GetSingleton()->IsOnceKeyDown('D')) 
		{
			installobj->SetOpen(true);
			player->SetDoor(true);
			player->DoorBreak();
		}
	}
	if (IntersectRect(&rcTemp2, &rcEnemy, &rcDoorAttack))
	{
		enemy->GetMonsterList()[index]->GetData()->isAlive = false;
		enemy->GetMonsterList()[index]->GetData()->shape = { -100,-100,-100,-100 };
	}
}


