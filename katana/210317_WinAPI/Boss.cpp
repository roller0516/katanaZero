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
	ImageManager::GetSingleton()->AddImage("boss_headhunter_no_head", "Image/Katana/boss/boss_nohead_6x2.bmp", 720, 88, 8, 2, true, RGB(255, 0, 255));

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
	data->size = data->image->GetImageInfo()->frameWidth;
	data->Name = "Bold";
	data->isAlive = true;
	data->attackAngle = 0;
	data->knockBackPower = 800;
	data->Index = 0;
	count = 0;

    dir = EnemyDir::Left;

	return S_OK;
}

void Boss::Release()
{
}

void Boss::Update()
{
	//for (int i = 0; i < 8; i++)
	//{
	//	enemyEffect[i].Update();
	//}

	data->localPos.x = data->worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
	data->localPos.y = data->worldPos.y - Camera::GetSingleton()->GetCameraPos().y;

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
		if (data->isAlive == false)
			data->currFrameX = data->maxFrame - 1;
	}


    if (data->target) 
    {
		data->attackAngle = GetAngle(this->data->worldPos, data->target->GetWorldpos());
    }

	if (data->isAlive)
	{
		data->shape.left = data->worldPos.x - data->size / 2;
		data->shape.top = data->worldPos.y - data->size / 2;
		data->shape.right = data->worldPos.x + data->size / 2;
		data->shape.bottom = data->worldPos.y + data->size / 2;
	}


	Pattern();

	PixelCollisionBottom();
	PixelCollisionRight();
	PixelCollisionLeft();

	if (data->isPhysic)
	{
		data->fallForce -= Gravity * TimerManager::GetSingleton()->GetElapsedTime() * data->velocity/* * TimerManager::GetSingleton()->GetElapsedTime()*/;
		data->worldPos.y -= data->fallForce * TimerManager::GetSingleton()->GetElapsedTime();
	}
}

void Boss::Render(HDC hdc,bool World)
{
    if (dir == EnemyDir::Left) 
    {
        data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
    }
    else 
    {
        data->image->FrameRender(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
    }
}

Enemy* Boss::Clone()
{
	return new Boss;
}

void Boss::Pattern()
{
    //srand((unsigned)time(NULL));
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
    default:
        break;
    }
       
    //Idle();
}

void Boss::SkyShoot()
{
    if (patterChange)
    {
        patterChange = false;
        patternIndex = 0;
        return;
    }

    if (index < 5 && start == false)
    {
        start = true;
        data->currFrameX = 0;
        state = BossState::TelePort_Fly;
        Animation(BossState::TelePort_Fly);
        data->isPhysic = false;
        data->worldPos.y = 200;
        data->worldPos.x = data->target->GetWorldpos().x;
        index++;
    }
    else if (index == 5) 
    {
        LastIndex = index;
    }  
}

void Boss::Dash()
{

    if (patterChange)
    {
        patterChange = false;
        patternIndex = 3;
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
            data->worldPos.x = 300;
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
        data->currFrameX = 0;
        state = BossState::DashEnd;
        Animation(BossState::DashEnd);
        LastIndex = index;
    } 

    float distance = sqrtf(powf(data->worldPos.x - currPos.x, 2) + powf(data->worldPos.y - currPos.y, 2));

    if (state == BossState::DashEnd && distance <= data->attackRange)
    {
        if (dir == EnemyDir::Left) 
        {
            data->worldPos.x -= 2000 * TimerManager::GetSingleton()->GetElapsedTime();
        }
        else
            data->worldPos.x += 2000 * TimerManager::GetSingleton()->GetElapsedTime();
    }
}


void Boss::Hurt(int index)
{
    if (index == 0) 
    {
        state = BossState::Recover;
        Animation(BossState::Recover);
    } 
    else 
    {
        state = BossState::RecoverHide;
        Animation(BossState::RecoverHide);
    }
}

void Boss::Idle()
{
    start = true;
    state = BossState::Idle;
    Animation(BossState::Idle);
}

void Boss::Die(bool isGround)
{
    if(isGround)
        Animation(BossState::DieGround);
    else
        Animation(BossState::DieFly);
}

void Boss::Grab()
{
    float angle;

    if (state == BossState::Jump)
    {
        if (dir == EnemyDir::Right)
        {
            angle = DegToRad(170);
            data->worldPos.x += cosf(angle) * 200 * TimerManager::GetSingleton()->GetElapsedTime();
        }
        else
        {
            angle = DegToRad(10);
            data->worldPos.x += cosf(angle) * 200 * TimerManager::GetSingleton()->GetElapsedTime();
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
            data->fallForce = 400;

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
            data->fallForce = 500;

        data->isPhysic = true;
        start = true;
        data->currFrameX = 0;
        state = BossState::WallJump;
        Animation(BossState::WallJump);
        index++;
    }

   
}

void Boss::DrawGun()
{
    Animation(BossState::DrawGun);
}

void Boss::TelePort()
{
    if (patterChange)
    {
        patterChange = false;
        patternIndex = 2;
        return;
    }
    int randNum;
    if (index < 3 && start == false)
    {
        if (data->target->GetWorldpos().x >= data->worldPos.x) 
        {
            randNum = (rand() % 50) + 20;
            dir = EnemyDir::Right;
        }
        else 
        {
            randNum = (rand() % 50) - 20;
            dir = EnemyDir::Right;
            dir = EnemyDir::Left;
        }
            
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

void Boss::Animation(BossState ani)
{
    stop = false;
    switch (ani)
    {
    case BossState::Idle:
        data->maxFrame = 12;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_idle");
        break;
    case BossState::Dash:
        data->maxFrame = 1;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_dash");
        break;
    case BossState::DashEnd:
        data->maxFrame = 10;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_dash_end");
        break;
    case BossState::PreDash:
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_predash");
        break;
    case BossState::Dead:
        data->maxFrame = 19;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_boss_dead");
        break;
    case BossState::DieFly:
        data->maxFrame = 4;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_diefly");
        break;
    case BossState::DieGround:
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_dieground");
        break;
    case BossState::Aim:
        data->maxFrame = 1;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_aim");
        break;
    case BossState::DrawGun:
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_drawgun");
        break;
    case BossState::Recover:
        data->maxFrame = 10;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_recover");
        break;
    case BossState::RecoverHide:
        data->maxFrame = 13;
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
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("boss_headhunter_no_head");
    case BossState::Head_Fly:
        head = ImageManager::GetSingleton()->FindImage("boss_headhunter_head_fly");
        break;
    case BossState::Head_Ground:
        head = head = ImageManager::GetSingleton()->FindImage("boss_headhunter_head_ground");
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
           //data->velocity = 100;
           //data->isTurn = false;
           //data->isPhysic = true;
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

