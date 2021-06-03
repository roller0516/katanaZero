#include "Boss.h"
#include "Camera.h"
#include "Image.h"
#include "Player.h"
#include "CommonFunction.h"
#include "MissileManager.h"
#include "AstarManager.h"
#include "EnemyEffect.h"
HRESULT Boss::Init(int posX, int posY)
{
	data = new Enemy::EnemyData;

    data->image =ImageManager::GetSingleton()->AddImage("boss_headhunter_idle", "Image/Katana/boss/boss_headhunter_idle_12x2.bmp", 528, 176, 12, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_dash", "Image/Katana/boss/boss_dash_1x2.bmp", 102, 100, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_dash_end", "Image/Katana/boss/boss_dash_end_10x2.bmp", 1000, 184, 10, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_boss_dead", "Image/Katana/boss/boss_dead_19x2.bmp", 2128, 84, 19, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_diefly", "Image/Katana/boss/boss_diefly_4x2.bmp", 328, 104, 4, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_dieground", "Image/Katana/boss/boss_dieground_8x2.bmp", 928, 164, 8, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_aim", "Image/Katana/boss/boss_headhunter_aim_1x2.bmp", 78, 208, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_land", "Image/Katana/boss/boss_land_4x2.bmp", 264, 172, 4, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_drawgun", "Image/Katana/boss/boss_headhunter_drawgun_8x2.bmp", 608, 172, 8, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_recover", "Image/Katana/boss/boss_headhunter_hurt_recover_10x2.bmp", 1400, 212, 10, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_recover_hide", "Image/Katana/boss/boss_headhunter_hurt_recover_13x2.bmp", 1820, 212, 13, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_in_pattern", "Image/Katana/boss/boss_in_pattern_4x2.bmp", 288, 124, 4, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_in_sweep", "Image/Katana/boss/boss_in_sweep_4x2.bmp", 432, 168, 4, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_out_pattern", "Image/Katana/boss/boss_out_pattern_4x2.bmp", 288, 124, 4, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_out_sweep", "Image/Katana/boss/boss_out_sweep_3x2.bmp", 264, 176, 3, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_predash", "Image/Katana/boss/boss_predash_8x2.bmp", 912, 172, 8, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_prejump", "Image/Katana/boss/boss_prejump_3x2.bmp", 198, 172, 3, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("boss_headhunter_jump", "Image/Katana/boss/boss_jump_1x2.bmp", 54, 176, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_shoot", "Image/Katana/boss/boss_shoot_8x2.bmp", 1088, 184, 8, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_teleport", "Image/Katana/boss/boss_teleport_8x2.bmp", 448, 180, 8, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_teleport_ground", "Image/Katana/boss/boss_teleport_ground_8x2.bmp", 656, 140, 8, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_wallgrab", "Image/Katana/boss/boss_wallgrab_3x2.bmp", 180, 172,3, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_walljump", "Image/Katana/boss/boss_walljump_7x2.bmp", 756, 248, 7, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_no_head", "Image/Katana/boss/boss_nohead_6x2.bmp", 720, 88, 6, 2, true, RGB(255, 0, 255));

    boss_warning = ImageManager::GetSingleton()->AddImage("boss_warning", "Image/Katana/dsBuffer.bmp", 907,2,1,1, true, RGB(255, 0, 255));
    lazerAim = ImageManager::GetSingleton()->AddImage("boss_aimlazer", "Image/Katana/boss/boss_aimlazer.bmp", 1000, 2,1,1, true, RGB(255, 0, 255));
	head = ImageManager::GetSingleton()->AddImage("boss_headhunter_head_fly", "Image/Katana/boss/boss_headfly_8x2.bmp", 320, 84, 8, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("boss_headhunter_head_ground", "Image/Katana/boss/boss_headground_8x2.bmp", 720, 56, 8, 2, true, RGB(255, 0, 255));

	data->isTurn = false;
	data->findRange = 300;
	data->attackRange = 200;
	data->worldPos.x = posX;
	data->worldPos.y = posY;
    data->isPhysic = true;
    data->velocity = 60;
	data->maxFrame = 8;
	data->moveSpeed = 1000;
    data->size = 70;
	data->Name = "Boss";
	data->isAlive = true;
	data->attackAngle = 0;
	data->knockBackPower = 800;
	data->Index = 0;
	count = 0;
    maxHp = 3;
    data -> hp = maxHp;
    dir = EnemyDir::Left;
    patternIndex = 1;
	return S_OK;
}

void Boss::Release()
{
}

void Boss::Update()
{
    if (shot)
    {
        attackCooltime += 20 * TimerManager::GetSingleton()->GetElapsedTime();
        if (attackCooltime > 0.8f)
        {
            if (missileCount < 20) 
            {
                missileCount++;
                attackCooltime = 0;
                if (dir == EnemyDir::Right)
                    data->attackAngle = DegToRad(-180 + missileCount * 8);
                else
                    data->attackAngle = DegToRad(-8 * missileCount);
                data->missileManager->Fire(data->attackAngle, data->worldPos.x +missileCount, data->worldPos.y, 0);
            }  
        }
    }


	data->localPos.x = data->worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
	data->localPos.y = data->worldPos.y - Camera::GetSingleton()->GetCameraPos().y;
    localHeadpos.x = headPos.x - Camera::GetSingleton()->GetCameraPos().x;
    localHeadpos.y = headPos.y - Camera::GetSingleton()->GetCameraPos().y;
	data->currFrameX += TimerManager::GetSingleton()->GetElapsedTime() * 10;

	if (data->currFrameX >= data->maxFrame)
	{
        if (LastIndex != 0) 
        {
            patterChange = true;
            index = 0;
            LastIndex = 0;
        }
        start = false;
        if (stop)
            data->currFrameX = data->maxFrame - 1;
        else
            data->currFrameX = 0;
	}


    if (data->target) 
    {
		data->attackAngle = GetAngle(this->data->worldPos, data->target->GetWorldpos());
    }

	if (data->isAlive)
	{
        if (data->isHit == false || state != BossState::TelePort_Ground) 
        {
            data->shape.left = data->worldPos.x - 44 / 2;
            data->shape.top = data->worldPos.y - 88 / 2;
            data->shape.right = data->worldPos.x + 44 / 2;
            data->shape.bottom = data->worldPos.y + 88 / 2;
        }
        Pattern();
	}
    else
        data->shape = { -1000,-1000,-1000,-1000 };
        

	PixelCollisionBottom();
	PixelCollisionRight();
	PixelCollisionLeft();

	if (data->isPhysic)
	{
        data->fallForce -= Gravity * TimerManager::GetSingleton()->GetElapsedTime() * data->velocity;
        headFallforce -= Gravity * TimerManager::GetSingleton()->GetElapsedTime() * data->velocity;
		data->worldPos.y -= data->fallForce * TimerManager::GetSingleton()->GetElapsedTime();
        headPos.y -= headFallforce * TimerManager::GetSingleton()->GetElapsedTime();
	}
}

void Boss::Render(HDC hdc,bool World)
{
    if (data->isAlive)
    {
        if (state == BossState::TelePort_Fly && aim)
            lazerAim->rotateRender(hdc, data->localPos.x, data->localPos.y+500, 0, 0, DegToRad(-90));
        if (dir == EnemyDir::Left)
        {
            if (state == BossState::PreDash || state == BossState::Dash && aim)
                boss_warning->Render(hdc, data->localPos.x- 900, data->localPos.y);
            if (state == BossState::Aim && aim)
                lazerAim->FrameRender(hdc, data->localPos.x-1000, data->localPos.y, 0, 0);

            if (state == BossState::RecoverHide)
                data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y - 10, data->currFrameX, 0, true);
            else if (state == BossState::Head_no)
                data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y + 20, data->currFrameX, 0, true);
            else if (state == BossState::Dead)
                data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y + 20, data->currFrameX, 0, true);
            else
                data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
            if (noHead)
                head->FrameRender(hdc, localHeadpos.x, localHeadpos.y, headFrame, 0, true);
        }
        else
        {
            if (state == BossState::PreDash || state == BossState::Dash && aim)
                boss_warning->Render(hdc, data->localPos.x, data->localPos.y);
            if (state == BossState::Aim && aim)
                lazerAim->FrameRender(hdc, data->localPos.x, data->localPos.y, 0, 0);

            if (state == BossState::RecoverHide)
                data->image->FrameRender(hdc, data->localPos.x, data->localPos.y - 10, data->currFrameX, 0, true);
            else if (state == BossState::Head_no)
                data->image->FrameRender(hdc, data->localPos.x, data->localPos.y + 20, data->currFrameX, 0, true);
            else if (state == BossState::Dead)
                data->image->FrameRender(hdc, data->localPos.x, data->localPos.y + 20, data->currFrameX, 0, true);
            else
                data->image->FrameRender(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
            if (noHead)
                head->FrameRender(hdc, localHeadpos.x, localHeadpos.y, headFrame, headFrame, 0, true);
        }
    }
    //Rectangle(hdc, data->shape.left- Camera::GetSingleton()->GetCameraPos().x, data->shape.top - Camera::GetSingleton()->GetCameraPos().y
    //    , data->shape.right - Camera::GetSingleton()->GetCameraPos().x, data->shape.bottom - Camera::GetSingleton()->GetCameraPos().y);
}

Enemy* Boss::Clone()
{
	return new Boss;
}

void Boss::Pattern()
{
    if (data->stage == 4) 
    {
        //if(data->isHit)
        DrawGunEasy();
        
        if(data->isHit)// 알파블랜딩
        {
            Hurt();
        }
    }

    if (data->stage == 5) 
    {
        float distance = Distance(data->worldPos, data->target->Getpos());

        NoHead();
        if (data->hp == 0)
        {
            if (isDie == false)
            {
                start = false;
                index = 0;
            }
            else
            {
                index = 1;
            }
            Die(isGround);
            return;
        }
        else if (data->hp > 0 && data->isHit && isGround)
        {
            data->isHit = false;
            start = false;
            index = 0;
            patternIndex = 6;
        }
        else if (data->hp > 0 && data->isHit && isGround == false)
        {
            data->isPhysic = true;
            data->isHit = false;
            start = false;
            index = 0;
            patternIndex = 6;
        }

        switch (patternIndex)
        {
        case 0:
            Dash();
            break;
        case 1:
            Grab();
            break;
        case 2:
            SkyShoot();
            break;
        case 3:
            TelePort();
            break;
        case 4:
            DrawGun();
            break;
        case 5:
            Shoot();
            break;
        case 6:
            Hurt();
            break;
        default:
            break;
        }
    }
}

void Boss::AttackBox(bool on)
{
    if (on)
    {
        data->attackShape.left = data->worldPos.x - data->size / 2;
        data->attackShape.top = data->worldPos.y - data->size / 2;
        data->attackShape.right = data->worldPos.x + data->size / 2;
        data->attackShape.bottom = data->worldPos.y + data->size / 2;
    }
    else
        data->attackShape = { -100,-100,-100,-100 };
}

void Boss::SkyShoot()
{
    
    int randNum = (rand() % 800)+300;
    if (data->isHit || isDie)
        return;
    if (patterChange)
    {
        patterChange = false;
        patternIndex = 0;
        return;
    }
   
    if (index < 5 && start == false)
    {
        aim = true;
        start = true;
        data->currFrameX = 0;
        state = BossState::TelePort_Fly;
        Animation(BossState::TelePort_Fly);
        if (data->currFrameX > 4)
            aim = false;
        data->isPhysic = false;
        data->worldPos.y = 200;
        data->worldPos.x = randNum;
        data->missileManager->TopLazer(DegToRad(0), data->worldPos.x, data->worldPos.y + 800,2);
        index++;
    }
    else if (index == 5) 
    {
        LastIndex = index;
    }  
}

void Boss::Dash()
{
    if (data->isHit || isDie)
        return;
    if (patterChange)
    {
        patterChange = false;
        patternIndex = 5;
        return;
    }
    data->isPhysic = true;
    data->attackRange = 800;
    if (index == 0 && start == false) 
    {
        
        if (WINSIZE_X / 2 > data->target->GetWorldpos().x)//left
        {
            data->worldPos.x = 1100;
            data->worldPos.y = 600;
        }
        else
        {
            data->worldPos.x = 250;
            data->worldPos.y = 600;
        }
        if (data->target->GetWorldpos().x >= data->worldPos.x)
            dir = EnemyDir::Right;
        else
            dir = EnemyDir::Left;

        start = true;
        data->currFrameX = 0;
        state = BossState::In_Pattern;
        Animation(BossState::In_Pattern);
        index++;
    }
    else if (index == 1 && start == false)
    {
        Idle();
        aim = true;
        start = true;
        data->currFrameX = 0;
        state = BossState::PreDash;
        Animation(BossState::PreDash);
        index++;
    }
    else if (index == 2 && start == false)
    {
        start = true;
        data->currFrameX = 0;
        state = BossState::Dash;
        Animation(BossState::Dash);
        index++;
    }
    else if (index == 3 && start == false)
    {
        currPos = data->worldPos;
        start = true;
        aim = false;
        data->currFrameX = 0;
        state = BossState::DashEnd;
        Animation(BossState::DashEnd);
        LastIndex = index;
    } 

    float distance = sqrtf(powf(data->worldPos.x - currPos.x, 2) + powf(data->worldPos.y - currPos.y, 2));

    if (state == BossState::DashEnd && distance <= data->attackRange)
    {
        AttackBox(true);
        if (dir == EnemyDir::Left) 
        {
            data->worldPos.x -= 2000 * TimerManager::GetSingleton()->GetElapsedTime();
        }
        else
            data->worldPos.x += 2000 * TimerManager::GetSingleton()->GetElapsedTime();
    }
}


void Boss::Hurt()
{
    if (patterChange)
    {
        patterChange = false;
        patternIndex = 3;
        return;
    }

    if (index == 0 && start == false)
    {
        start = true;
        data->currFrameX = 0;
        index++;
    }

    RecoverHide();
}

void Boss::Idle()
{
    start = true;
    state = BossState::Idle;
    Animation(BossState::Idle);
}

void Boss::Die(bool isGround)
{
    if (index == 1 && isDie && start == false)
    {
        start = true;
        data->isHit = false;
        state = BossState::Dead;
        Animation(BossState::Dead);
    }
    else if (isDie == false && isGround && start == false)
    {
        isDie = true;
        start = true;
        data->currFrameX = 0;
        state = BossState::DieGround;
        Animation(BossState::DieGround);
    }  
    else if (isDie == false && isGround == false && start == false)
    {
        data->isPhysic = true;
        data->fallForce = 100;
        isDie = true;
        start = true;
        data->currFrameX = 0;
        state = BossState::DieFly;
        Animation(BossState::DieFly);
    } 

    if (state == BossState::Dead)
    {    
        if (data->target->GetWorldpos().x >= data->worldPos.x)
        {
            dir = EnemyDir::Right;
            data->worldPos.x += 10 * TimerManager::GetSingleton()->GetElapsedTime();
        }
        else
        {
            dir = EnemyDir::Left;
            data->worldPos.x -= 10 * TimerManager::GetSingleton()->GetElapsedTime();
        }
    }
}

void Boss::Grab()
{
    if (data->isHit|| isDie)
        return;

    float angle;
    float attackAngle;
    
    if (state == BossState::Jump)
    {
        isGround = false;
        if (dir == EnemyDir::Right)
        {
            angle = DegToRad(170);
            data->worldPos.x += cosf(angle) * 800 * TimerManager::GetSingleton()->GetElapsedTime();
        }
        else
        {
            angle = DegToRad(10);
            data->worldPos.x += cosf(angle) * 500 * TimerManager::GetSingleton()->GetElapsedTime();
        }
    }

    if (state == BossState::WallJump)
    {
        if (dir == EnemyDir::Right)
        {
            angle = DegToRad(120);
            data->worldPos.x += cosf(angle) * 800 * TimerManager::GetSingleton()->GetElapsedTime();
        }
        else
        {
            angle = DegToRad(60);
            data->worldPos.x += cosf(angle) * 800 * TimerManager::GetSingleton()->GetElapsedTime();
        }
    }


    if (index == 4)
        return;
    if (patterChange)
    {
        patterChange = false;
        patternIndex = 2;
        return;
    }
    if (index== 0 && start == false)
    {
        //Idle();
        if (data->target->GetWorldpos().x >= data->worldPos.x)
            dir = EnemyDir::Right;
        else
            dir = EnemyDir::Left;
        start = true;
        data->currFrameX = 0;
        state = BossState::PreJump;
        Animation(BossState::PreJump);
        index++;
    }
    else if (index == 1 && start == false)
    {
        if (index == 1)
            data->fallForce = 500;

        start = true;
        data->currFrameX = 0;
        state = BossState::Jump;
        Animation(BossState::Jump);
        index++;
    }
    else if (index == 2 && start == false && (data->rightWall || data->leftWall))
    {
        if (dir == EnemyDir::Right)
            dir = EnemyDir::Left;
        else if(dir == EnemyDir::Left)
            dir = EnemyDir::Right;
        start = true;
        data->currFrameX = 0;
        state = BossState::WallGrab;
        data->isPhysic = false;
        Animation(BossState::WallGrab);
        index++;
    }
    else if (index == 3 && start == false)
    {
        if (dir == EnemyDir::Right)
            dir = EnemyDir::Right;
        else if (dir == EnemyDir::Left)
            dir = EnemyDir::Left;

        if (index == 3)
            data->fallForce = 600;

        missileCount = 0;
        data->isPhysic = true;
        start = true;
        data->currFrameX = 0;
        state = BossState::WallJump;
        Animation(BossState::WallJump);
        shot = true;
        index++;
    }

    
}

void Boss::DrawGun()
{
    if (data->isHit)
        return;

    if (data->stage == 5) 
    {
        if (patterChange)
        {
            patterChange = false;
            patternIndex = 3;
            aim = false;
            return;
        }
    }

    if (index == 0 && start == false)
    {
        if (WINSIZE_X / 2 > data->target->GetWorldpos().x)//left
        {
            data->worldPos.x = 1000;
            data->worldPos.y = 600;
        }
        else
        {
            data->worldPos.x = 300;
            data->worldPos.y = 600;
        }
        start = true;
        data->currFrameX = 0;
        state = BossState::In_Pattern;
        Animation(BossState::In_Pattern);
        index++;

        if (data->target->GetWorldpos().x >= data->worldPos.x)
            dir = EnemyDir::Right;
        else
            dir = EnemyDir::Left;
    }
    else if (index == 1 && start == false)
    {
        if(dir == EnemyDir::Left)
            data->missileManager->Lazer(DegToRad(180), data->worldPos.x-790 , data->worldPos.y,1.5f);
        if (dir == EnemyDir::Right)
            data->missileManager->Lazer(DegToRad(0), data->worldPos.x+ 790, data->worldPos.y,1.5f);
        start = true;
        data->currFrameX = 0;
        state = BossState::DrawGun;
        Animation(BossState::DrawGun);
        index++;
    }
    else if (index == 2 && start == false)
    {
        aim = true;
        start = true;
        data->currFrameX = 0;
        state = BossState::Aim;
        Animation(BossState::Aim);
        index++;
        LastIndex = index;
    }
}

void Boss::DrawGunEasy()
{
    if (data->isHit)
        return;

    if (index == 0 && start == false)
    {
        if (index == 0) 
        {
            if (WINSIZE_X / 2 > data->target->GetWorldpos().x)//left
            {
                data->worldPos.x = 1000;
                data->worldPos.y = 900;
            }
            else
            {
                data->worldPos.x = 250;
                data->worldPos.y = 900;
            }
        }
        start = true;
        data->currFrameX = 0;
        index++;

        if (data->target->GetWorldpos().x >= data->worldPos.x)
            dir = EnemyDir::Right;
        else
            dir = EnemyDir::Left;
    }
    else if (index == 1 && start == false)
    {
        start = true;
        data->currFrameX = 0;
        state = BossState::DrawGun;
        Animation(BossState::DrawGun);
        index++;
    }
    else if (index == 2 && start == false)
    {
        //if (dir == EnemyDir::Left)
        //    data->missileManager->Lazer(DegToRad(180), data->worldPos.x - 790, data->worldPos.y, 1.5f);
        //if (dir == EnemyDir::Right)
        //    data->missileManager->Lazer(DegToRad(0), data->worldPos.x + 790, data->worldPos.y, 1.5f);

        aim = true;
        start = true;
        data->currFrameX = 0;
        state = BossState::Aim;
        Animation(BossState::Aim);

        if (data->isHit) 
        {
            index++;
            LastIndex = index;
        }
    }

}

void Boss::TelePort()
{
    int randNum = rand() % 5;
    data->shape = { -100,-100,-100,-100 };
    if (patterChange)
    {
        patterChange = false;
        patternIndex = randNum;
        return;
    }

    if (index < 3 && start == false)
    {
        if (index == 0) 
        {
            if (data->target->GetWorldpos().x >= data->worldPos.x)
                dir = EnemyDir::Right;
            else
                dir = EnemyDir::Left;
        }
        

        if (data->target->GetWorldpos().x >= data->worldPos.x) 
            randNum = (rand() % 30) + 10;
        else 
            randNum = (rand() % 30) - 10;
            
        start = true;
        data->currFrameX = 0;
        state = BossState::Out_Pattern;
        Animation(BossState::Out_Pattern);
        data->worldPos.x += randNum;
        index++;
    }
    else if (index == 3)
    {
        LastIndex = index;
    }
}

void Boss::Shoot()
{
    if (data->isHit || isDie)
        return;

    if (patterChange)
    {
        patterChange = false;
        patternIndex = 4;
        return;
    }

    if (index < 2 && start == false)
    {
        if (index == 0) 
        {
            if (WINSIZE_X / 2 > data->target->GetWorldpos().x)//left
            {
                data->worldPos.x = 1000;
                data->worldPos.y = 600;
            }
            else
            {
                data->worldPos.x = 300;
                data->worldPos.y = 600;
            }
            if (data->target->GetWorldpos().x >= data->worldPos.x)
                dir = EnemyDir::Right;
            else
                dir = EnemyDir::Left;
        }
        if (dir == EnemyDir::Right) 
        {
            data->attackAngle = (rand() % 30) + 10;
            data->missileManager->Mine(DegToRad(data->attackAngle += 15), data->worldPos.x, data->worldPos.y, 0.5f);
        }
        else 
        {
            data->attackAngle = (rand() % 30) + 140;
            data->missileManager->Mine(DegToRad(data->attackAngle -= 15), data->worldPos.x, data->worldPos.y, 0.5f);
        }
            
        start = true;
        data->currFrameX = 0;
        state = BossState::Shoot;
        Animation(BossState::Shoot);
        index++;
    }
    else if (index == 2)
    {
        LastIndex = index;
    }
}

void Boss::RecoverHide()
{
    state = BossState::RecoverHide;
    Animation(BossState::RecoverHide);
}

void Boss::Recover()
{
    Animation(BossState::Recover);
}

void Boss::TelePort_Ground()
{
    Animation(BossState::TelePort_Ground);
}

void Boss::Aim()
{
    Animation(BossState::Aim);
}

void Boss::NoHead()
{
    if (data->isHit && state == BossState::Dead)
    {   
        noHead = true;
        start = true;
        data->currFrameX = 0;
        state = BossState::Head_no;
        Animation(BossState::Head_no);
        headPos = data->worldPos;
        headFrame = 0;
        headFallforce = 200;
    }
    if (noHead) 
    {
        headFrame += 15*TimerManager::GetSingleton()->GetElapsedTime();
        if (headFrame > headMaxFrame) 
        {
            headFrame = 0;
        }
    }
}


void Boss::changePattern(int index)
{
    
}


void Boss::Animation(BossState ani)
{
    shot = false;
    switch (ani)
    {
    case BossState::Idle:
        stop = false;
        data->maxFrame = 12;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_idle");
        break;
    case BossState::Dash:
        stop = false;
        data->maxFrame = 1;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_dash");
        break;
    case BossState::DashEnd:
        stop = false;
        data->maxFrame = 10;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_dash_end");
        break;
    case BossState::PreDash:
        stop = false;
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_predash");
        break;
    case BossState::Dead:
        stop = false;
        data->maxFrame = 19;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_boss_dead");
        break;
    case BossState::DieFly:
        stop = true;
        data->maxFrame = 4;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_diefly");
        break;
    case BossState::DieGround:
        stop = true;
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_dieground");
        break;
    case BossState::Aim:
        stop = false;
        data->maxFrame = 1;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_aim");
        break;
    case BossState::DrawGun:
        stop = true;
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_drawgun");
        break;
    case BossState::Recover:
        data->maxFrame = 10;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_recover");
        break;
    case BossState::RecoverHide:
        if (isGround == false && index == 1) 
        {
            data->maxFrame = 3;
            stop = true;
            index++;
        }
        else if (isGround && index==2)
        {
            data->maxFrame = 13;
            data->currFrameX = 3;
            index++;
            LastIndex = index;
        }
        else if (isGround && index == 1) 
        {
            if (data->stage == 4) 
            {
                data->isHit = false;
                data->isAlive = false;
            }
            data->maxFrame = 13;
            LastIndex = index;
        }
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_recover_hide");
        break;
    case BossState::In_Pattern:
        data->maxFrame = 4;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_in_pattern");
        break;
    case BossState::In_Sweep:
        data->maxFrame = 4;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_in_sweep");
        break;
    case BossState::Out_Pattern:
        data->maxFrame = 4;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_out_pattern");
        break;
    case BossState::Out_Sweep:
        data->maxFrame = 4;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_out_sweep");
        break;
    case BossState::PreJump:
        data->maxFrame = 3;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_prejump");
        break;
    case BossState::Shoot:
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_shoot");
        break;
    case BossState::TelePort_Fly:
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_teleport");
        break;
    case BossState::TelePort_Ground:
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_teleport_ground");
        break;
    case BossState::WallGrab:
        data->maxFrame = 3;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_wallgrab");
        break;
    case BossState::WallJump:
        stop = true;
        data->maxFrame = 7;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_walljump");
        break;
    case BossState::Head_no:
        stop = true;
        data->maxFrame = 6;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_no_head");
    case BossState::Head_Fly:
        headMaxFrame = 8;
        head = ImageManager::GetSingleton()->FindImage("boss_headhunter_head_fly");
        break;
    case BossState::Head_Ground:
        head = ImageManager::GetSingleton()->FindImage("boss_headhunter_head_ground");
        break;
    case BossState::Land:
        stop = true;
        data->maxFrame = 4;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_land");
        break;
    case BossState::Jump:
        data->maxFrame = 1;
        head = head = ImageManager::GetSingleton()->FindImage("boss_headhunter_jump");
        break;
    default:
        break;
    }
}

void Boss::PixelCollisionBottom()
{
    if (state == BossState::Jump)
        return;
    COLORREF color;
    int R, G, B;
    float playerHeight = 40;
    float currPosBottom = data->worldPos.y + playerHeight;
    for (int i = currPosBottom - 5; i < currPosBottom + 5; i++)
    {
        color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
            data->worldPos.x, i);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);

        if (!(R == 255 && G == 0 && B == 255))
        {

            if (state == BossState::WallJump) 
            {
                data->currFrameX = 0;
                state = BossState::Land;
                Animation(BossState::Land);
                LastIndex = index;
            }

            data->fallForce = 0;
            isGround = true;
            if (!data->isAlive)
            {
                data->worldPos.y = i - playerHeight;
                break;
            }

            data->worldPos.y = i - playerHeight - 5;
            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
        {
            isGround = false;
        }
    }
}
void Boss::PixelCollisionLeft()
{
    COLORREF color;
    int R, G, B;
    float playerwidth = 35;
    float currPosLeft = data->worldPos.x - playerwidth;
    for (int i = currPosLeft; i < currPosLeft + 2; i++)
    {
        color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
            i, data->worldPos.y);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);
        if (!(R == 255 && G == 0 && B == 255))
        {
            data->worldPos.x = i + playerwidth + 2;
            data->leftWall = true;
            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
            data->leftWall = false;
    }
}

void Boss::PixelCollisionRight()
{
    COLORREF color;
    int R, G, B;
    float playerwidth = 35;
    float currPosRight = data->worldPos.x + playerwidth;
    for (int i = currPosRight - 2; i < currPosRight; i++)
    {
        color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
            i, data->worldPos.y);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);
        if (!(R == 255 && G == 0 && B == 255))
        {

            data->worldPos.x = i - playerwidth - 2;
            data->rightWall = true;
            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
            data->rightWall = false;
    }
}

