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
#include "Boss.h"
#include "GameNode.h"

void CollisionManager::MissilePlayerEnemy(MissileManager* missile, Player* player, EnemyManager* enemy, GameNode* battleScene, int index)
{
	RECT rcTemp, rcplayer, rcmissile,rcReflect,rcEnemy;
	rcplayer = player->GetRect();
	rcReflect = player->GetAttackShape();
	rcEnemy = enemy->GetMonsterList()[index]->GetData()->shape;
	for (int i = 0; i < missile->GetMissile().size(); i++) 
	{
		rcmissile = missile->GetMissile()[i]->GetRect();

		if (missile->GetMissile()[i]->GetIsFired() == true 
			&& missile->GetMissile()[i]->GetOwnerType() == MissileOwnerType::enemy
			&& IntersectRect(&rcTemp,&rcplayer,&rcmissile))
		{
			if (IntersectRect(&rcTemp, &rcReflect, &rcmissile))
			{
				missile->GetMissile()[i]->SetAngle(player->GetPlayerAngle());
				missile->GetMissile()[i]->SetOwnerType(MissileOwnerType::player);
			}
			missile->GetMissile()[i]->SetPos(-100, -100);
			missile->GetMissile()[i]->SetIsFired(false);
			break;
		}

		if (missile->GetMissile()[i]->GetIsFired() == true
			&& missile->GetMissile()[i]->GetOwnerType() == MissileOwnerType::player
			&& IntersectRect(&rcTemp, &rcEnemy, &rcmissile)) //�ݻ�
		{
			TimerManager::GetSingleton()->SetTimeStop(true);
			player->ReflectEffect();
			player->HitEffect(enemy->GetMonsterList()[index]->GetData()->worldPos.x,
			enemy->GetMonsterList()[index]->GetData()->worldPos.y);
			enemy->GetMonsterList()[index]->GetData()->isAlive = false;
			enemy->GetMonsterList()[index]->GetData()->shape = { -100,-100,-100,-100 };
			battleScene->SetMonsterCount(battleScene->GetMonsterCount() - 1);
			missile->GetMissile()[i]->SetPos(-100, -100);
			missile->GetMissile()[i]->SetIsFired(false);
			break;
		}
	}
}


void CollisionManager::EnemyPlayer(EnemyManager* enemy, Player* player, GameNode* battleScene, int index)
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
		battleScene->SetMonsterCount(battleScene->GetMonsterCount() - 1);
	}
}

void CollisionManager::EnemyItem(Player* player, EnemyManager* enemy, GameNode* battleScene, ItemManager* item, int index)
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
		battleScene->SetMonsterCount(battleScene->GetMonsterCount() - 1);
	}
}

void CollisionManager::PlayerDoorEnemy(Player* player, InstallObject* installobj, GameNode* battleScene, EnemyManager* enemy, int index)
{
	RECT rcTemp, rcTemp2 , rcTemp3,rcPlayer, rcDoor, rcEnemy, rcDoorAttack;
	FPOINT currPos = player->GetWorldpos();

	rcPlayer = player->GetRect();
	rcDoor = installobj->GetShape();
	rcDoorAttack = installobj->GetAttackShape();
	rcEnemy = enemy->GetMonsterList()[index]->GetData()->shape;

	if (IntersectRect(&rcTemp, &rcPlayer, &rcDoor) && installobj->GetClose())
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown('D')) 
		{
			installobj->SetOpen(true);
			player->SetDoor(true);
			player->DoorBreak();
		}
	}
	else if (rcPlayer.right-1 == rcDoor.left)
	{
		player->SetPos(currPos.x - 1, currPos.y - 1);
	}
	else if (rcPlayer.left-1 == rcDoor.right)
	{
		player->SetPos(currPos.x + 1, currPos.y + 1);
	}

	if (IntersectRect(&rcTemp3, &rcEnemy, &rcDoorAttack))
	{
		enemy->GetMonsterList()[index]->GetData()->isAlive = false;
		enemy->GetMonsterList()[index]->GetData()->shape = { -100,-100,-100,-100 };
		battleScene->SetMonsterCount(battleScene->GetMonsterCount() - 1);
	}
	if (rcEnemy.right == rcDoor.left)
	{
		enemy->GetMonsterList()[index]->GetData()->rightWall = true;
		enemy->GetMonsterList()[index]->GetData()->worldPos.x -= 1;
		enemy->GetMonsterList()[index]->GetData()->worldPos.y -= 1;
	}
	else if (rcEnemy.left == rcDoor.right)
	{
		enemy->GetMonsterList()[index]->GetData()->leftWall = true;
		enemy->GetMonsterList()[index]->GetData()->worldPos.x += 1;
		enemy->GetMonsterList()[index]->GetData()->worldPos.y += 1;
	}
}

void CollisionManager::BossPlayer(Player* player, Enemy* boss)
{
	RECT rcTemp, rcTemp2, rcPlayer, rcBoss,rcPlayerAttack,rcBossAtttack;

	rcPlayer = player->GetRect();
	rcBoss = boss->GetData()->shape;
	rcPlayerAttack = player->GetAttackShape();
	rcBossAtttack = boss->GetData()->attackShape;
	if (IntersectRect(&rcTemp, &rcPlayerAttack, &rcBoss) && count == 0)
	{
		count++;
		Camera::GetSingleton()->Shake(0.2f);
		player->SetAttackShape();
		boss->GetData()->shape = { -200,-200,-200,-200 };
		boss->GetData()->isHit = true;
		boss->GetData()->hp -= 1;
	}
	else if(count == 1)
		count = 0;

	if (IntersectRect(&rcTemp2, &rcPlayer, &rcBossAtttack))
	{
		player->Die();
	}
}




