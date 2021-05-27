#include "Enemy_Bold.h"
#include "Camera.h"
#include "Image.h"
#include "Player.h"
#include "CommonFunction.h"
#include "MissileManager.h"
#include "AstarManager.h"
HRESULT Enemy_Bold::Init(int posX, int posY)
{
    data = new Enemy::EnemyData;
	data->image = ImageManager::GetSingleton()->AddImage("Bold_Idle", "Image/Katana/enemy/enemy_bold_Idle_8x2.bmp", 576, 140, 8, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Bold_hurt", "Image/Katana/enemy/enemy_bold_hurt_14x2.bmp", 1036, 128, 14, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Bold_run", "Image/Katana/enemy/enemy_bold_run_10x2.bmp", 900, 156, 10, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Bold_walk", "Image/Katana/enemy/enemy_bold_walk_8x2.bmp", 544, 152, 8, 2, true, RGB(255, 0, 255));
    armLImage = ImageManager::GetSingleton()->AddImage("Bold_arm_l", "Image/Katana/enemy/enemy_bold_leftarm_R.bmp", 28, 16, 1, 1,true, RGB(255, 0, 255));
    armRImage = ImageManager::GetSingleton()->AddImage("Bold_arm_r", "Image/Katana/enemy/enemy_bold_rightarm_R.bmp", 30, 18, 1, 1,true, RGB(255, 0, 255));
    GunImage = ImageManager::GetSingleton()->AddImage("Bold_gun", "Image/Katana/enemy/enemy_bold_gun.bmp", 54, 12, 1, 1,true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Bold_attack", "Image/Katana/enemy/enemy_bold_aim_R.bmp", 42, 70, 1, 1, true, RGB(255, 0, 255));
    missileManager = new MissileManager();
    missileManager->Init(this);
    data->astar = new AstarManager();
    data->astar->Init();
    data->astar->SetOnwer(this);
    data->findRange = 200;
    data->attackRange = 300;
    data->worldPos.x = posX;
    data->worldPos.y = posY;
    data->maxFrame = 8;
    data->moveSpeed = 200;
    data->size = data->image->GetImageInfo()->frameWidth;
    data->Name = "Bold";
    data->isAlive = true;
    data->attackAngle = 0;
    data->knockBackPower = 800;
	return S_OK;
}

void Enemy_Bold::Release()
{
    SAFE_RELEASE(missileManager);
}

void Enemy_Bold::Update()
{
    missileManager->Update();

    data->attackAngle = GetAngle(this->data->worldPos, data->target->GetWorldpos());

    data->astar->Update();

    data->localPos.x = data->worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
    data->localPos.y = data->worldPos.y - Camera::GetSingleton()->GetCameraPos().y;

    data->currFrameX += TimerManager::GetSingleton()->GetElapsedTime() * 10;
    if (data->currFrameX >= data->maxFrame) 
    {
        data->currFrameX = 0;
        if (data->isAlive == false)
            data->currFrameX = data->maxFrame-1;
    }
        
    
    if (data->isAlive)
    {
        data->shape.left = data->localPos.x - data->size / 2;
        data->shape.top = data->localPos.y - data->size / 2;
        data->shape.right = data->localPos.x + data->size / 2;
        data->shape.bottom = data->localPos.y + data->size / 2;
    }
    if (data->isPhysic&&data->isSamPle ==false)
    {
        data->fallForce -= Gravity * TimerManager::GetSingleton()->GetElapsedTime() * data->velocity/* * TimerManager::GetSingleton()->GetElapsedTime()*/;
        data->worldPos.y -= data->fallForce * TimerManager::GetSingleton()->GetElapsedTime();
    }
    if (data->isSamPle == false) 
    {
        Pattern();
        if (data->isKnockBack)
            KnockBack();
        PixelCollisionBottom();
        PixelCollisionRight();
        PixelCollisionLeft();
    }
}

void Enemy_Bold::Render(HDC hdc, bool world)
{
    //data->astar->Render(hdc);

        if (world)
        {
            if ((dir == EnemyDir::Left)) 
            {
                if (data->isAttack)
                {
                    armLImage->FrameRenderFlip(hdc, data->worldPos.x - 20, data->worldPos.y - 10, 0, 0, true);
                    data->image->FrameRenderFlip(hdc, data->worldPos.x, data->worldPos.y, data->currFrameX, 0, true);
                    GunImage->FrameRenderFlip(hdc, data->worldPos.x - 20, data->worldPos.y -12, 0, 0, true);
                    armRImage->FrameRenderFlip(hdc, data->worldPos.x, data->worldPos.y - 10, 0, 0, true);
                }
                else
                    data->image->FrameRenderFlip(hdc, data->worldPos.x, data->worldPos.y, data->currFrameX, 0, true);
            } 
            else 
            {
                if (data->isAttack)
                {
                    armLImage->FrameRender(hdc, data->worldPos.x + 20, data->worldPos.y - 10, 0, 0, true);
                    data->image->FrameRender(hdc, data->worldPos.x, data->worldPos.y, data->currFrameX, 0, true);
                    GunImage->FrameRender(hdc, data->worldPos.x + 20, data->worldPos.y - 12, 0, 0, true);
                    armRImage->FrameRender(hdc, data->worldPos.x, data->worldPos.y - 10, 0, 0, true);
                }
                else
                    data->image->FrameRender(hdc, data->worldPos.x, data->worldPos.y, data->currFrameX, 0, true);
            }    
        }
        else 
        {
            if (data->localPos.x > WINSIZE_X || data->localPos.y >WINSIZE_Y)
                return;
            missileManager->Render(hdc);
            if (dir == EnemyDir::Left) 
            {
                if (data->isAttack)
                {
                    armLImage->rotateRenderFlip(hdc, data->localPos.x - 20, data->localPos.y - 10, 0, 0, data->attackAngle);
                    data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
                    GunImage->rotateRenderFlip(hdc, data->localPos.x -10, data->localPos.y - 10, 0, 0, data->attackAngle);
                    armRImage->rotateRenderFlip(hdc, data->localPos.x+5, data->localPos.y-10, 0, 0, data->attackAngle);
                }
                else if (state == EnemyState::hurt)
                    data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y + 12, data->currFrameX, 0, true);
                else
                    data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
            }  
            else 
            {
                if (data->isAttack)
                {
                    armLImage->rotateRender(hdc, data->localPos.x + 20, data->localPos.y - 10,0,0, data->attackAngle);
                    data->image->FrameRender(hdc, data->localPos.x, data->localPos.y, data->currFrameX,0, true);
                    GunImage->rotateRender(hdc, data->localPos.x+10, data->localPos.y-10,0,0, data->attackAngle);
                    armRImage->rotateRender(hdc, data->localPos.x-5, data->localPos.y-10,0,0, data->attackAngle);
                }
                else if (state == EnemyState::hurt)
                    data->image->FrameRender(hdc, data->localPos.x, data->localPos.y + 12, data->currFrameX, 0, true);
                else
                    data->image->FrameRender(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
            }  
            
        }
}

Enemy* Enemy_Bold::Clone()
{
	return new Enemy_Bold;
}

void Enemy_Bold::Pattern()
{
    if (data->isAlive == false) 
    {
        data->isKnockBack = true;
        Die();
        return;
    }
    if (data->target == nullptr) return;
    float distance = Distance(this->data->worldPos, data->target->GetWorldpos());
    
    if (data->attackRange > distance && data->isFind) // 찾았고 공격 거리에있을때
    {
        if (data->attackAngle < DegToRad(30) && data->attackAngle > DegToRad(-30))
        {
            data->isAttack = true;
            dir = EnemyDir::Right;
            Attack(dir);
            return;
        }
        else if (data->attackAngle >= DegToRad(150) && data->attackAngle <= DegToRad(180) || 
            data->attackAngle <= DegToRad(-150) && data->attackAngle >= DegToRad(-180))
        {
            data->isAttack = true;
            dir = EnemyDir::Left;
            Attack(dir);
            return;
        }
    }
    data->isAttack = false;
    //찾았을때 무조건 내위치로 따라옴
    if (data->isFind) 
    {
        data->delay = 0;
        Run();
        return;
    }

    if (data->findRange > distance)
        data->isFind = true;
     //거리로 찾을때
    Idle();

    data->delay += TimerManager::GetSingleton()->GetElapsedTime();
    if (data->delay > 5)
    {
        if (data->delay > 8) 
        {
            data->delay = 0;
            Idle();
            return;
        }
        Walk();
        return;
    }
}

void Enemy_Bold::Attack(EnemyDir dir)
{
   //data->astar->Clear();
   targeton = 0;
   data->attackSpeed += TimerManager::GetSingleton()->GetElapsedTime();
   if (data->attackSpeed > 1) 
   {
       data->attackSpeed = 0;
       if(dir == EnemyDir::Left)
           missileManager->Fire(data->attackAngle,data->worldPos.x - 5, data->worldPos.y -10);
       else
           missileManager->Fire(data->attackAngle, data->worldPos.x + 5, data->worldPos.y - 10);
   }
   state = EnemyState::attack;
   Animation(state);
}

void Enemy_Bold::Run()
{
    if (data->isSamPle)
        data->moveSpeed = 0;
    else
        data->moveSpeed = 150;
    FPOINT bottomPos;
    bottomPos.x = data->worldPos.x - 20;
    bottomPos.y = data->worldPos.y + 28;
    for (int i = 0; i < 4; i++)
    {
        if (destAngle < 0 && data->astar->GetTileIndex().x == 44 + i && data->astar->GetTileIndex().y == 76)
        {
            data->angle = 0;
        }
    }
    if (data->astar->GetParentList().size() > 0)
    {
        targeton++;
    }
    if (data->astar->GetBackTile())
    {
        if (targeton == 1)
        {
            targeton = 2;
            destAngle = GetAngle(bottomPos, data->astar->GetDestTile()->GetCenter());
        }

        if (data->astar->GetBackTile()->GetIdX() == data->astar->GetTileIndex().x &&
            data->astar->GetBackTile()->GetIdY() == data->astar->GetTileIndex().y)
        {

            if (data->astar->GetParentList().size() == 0)
            {
                data->astar->SetBackTile(data->astar->GetDestTile());
                targeton = 0;
                data->angle = 0;
            }
            else
            {
                data->astar->ParentPopBack();
                if (data->astar->GetParentList().size() > 0)
                {
                    int a = data->astar->GetParentList().size();
                    data->astar->SetBackTile(data->astar->GetParentList().back());
                }
            }
            data->angle = GetAngle(bottomPos, data->astar->GetBackTile()->GetCenter());
            if (data->angle > DegToRad(90) || data->angle < DegToRad(-90))
            {
                dir = EnemyDir::Left;
            }
            else if (data->angle <= DegToRad(90) || data->angle <= DegToRad(-90))
            {
                dir = EnemyDir::Right;
            }
        }
        data->worldPos.x += cosf(data->angle) * data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
        data->worldPos.y -= sinf(data->angle) * data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    }
    state = EnemyState::run;
    Animation(state);
}

void Enemy_Bold::Walk()
{
    if (data->leftWall)
        dir = EnemyDir::Right;
    else if (data->rightWall)
        dir = EnemyDir::Left;

    if (data->isSamPle)
        data->moveSpeed = 0;
    else
        data->moveSpeed = 100;

    if(dir == EnemyDir::Right)
        data->worldPos.x += data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    else
        data->worldPos.x -= data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    state = EnemyState::walk;
    Animation(state);
}

void Enemy_Bold::Hurt()
{
    data->moveSpeed = 0;
    state = EnemyState::hurt;
    Animation(state);
}

void Enemy_Bold::Idle()
{
    data->moveSpeed = 0;
    state = EnemyState::idle;
    Animation(state);
}

void Enemy_Bold::Die()
{
    if (data->isAlive == false) 
    {
        data->isFind = false;
        data->isAttack = false;
        state = EnemyState::hurt;
        Animation(state);
    }
}

void Enemy_Bold::KnockBack()
{
    float angle = data->target->GetPlayerAngle();
    if (data->knockBackPower <= 0)
    {
        data->knockBackPower = 0;
        data->isKnockBack = false;
        return;
    }
    
    data->knockBackPower -= 100 * TimerManager::GetSingleton()->GetElapsedTime();
    data->worldPos.x += cosf(angle) * data->knockBackPower * TimerManager::GetSingleton()->GetElapsedTime();
    data->worldPos.y -= sinf(angle) * data->knockBackPower * TimerManager::GetSingleton()->GetElapsedTime();
}


void Enemy_Bold::Animation(EnemyState ani)
{
    switch (ani)
    {
    case EnemyState::idle:
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("Bold_Idle");
        break;
    case EnemyState::walk:
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("Bold_walk");
        break;
    case EnemyState::run:
        data->maxFrame = 10;
        data->image = ImageManager::GetSingleton()->FindImage("Bold_run");
        break;
    case EnemyState::attack:
        data->maxFrame = 1;
        data->image = ImageManager::GetSingleton()->FindImage("Bold_attack");
        break;
    case EnemyState::hurt:
        data->maxFrame = 14;
        data->image = ImageManager::GetSingleton()->FindImage("Bold_hurt");
        break;
    }
}
void Enemy_Bold::PixelCollisionBottom()
{
    COLORREF color;
    int R, G, B;
    float playerHeight = 34;

    float currPosBottom = data->worldPos.y + playerHeight;
    for (int i = currPosBottom-10; i < currPosBottom+10; i++)
    {
        color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
            data->worldPos.x, i);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);

        if (!(R == 255 && G == 0 && B == 255))
        {
            if (data->angle < DegToRad(0)&& destAngle < DegToRad(0)&&(R == 0 && G == 0 && B == 0))
                break;
            data->velocity = 60;
            data->fallForce = 0;
            if (data->isKnockBack)
            {
                //data->fallForce = 100;
                data->isKnockBack= false;
                //data->worldPos.y = i - playerHeight - 20;
                break;
            }
            if (data->isAlive == false)
            {
                break;
            }
            
            data->worldPos.y = i - playerHeight -2;
            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
        {
            
        }
    }
}
void Enemy_Bold::PixelCollisionLeft()
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
            if (data->isKnockBack) 
            {
                data->knockBackPower = 0;
                data->isKnockBack = false;
                break;
            }
            if (R == 0 && G == 0 && B == 0)break;
            data->worldPos.x = i + playerwidth+5;
            data->leftWall = true;
            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
            data->leftWall = false;
    }
}

void Enemy_Bold::PixelCollisionRight()
{
    COLORREF color;
    int R, G, B;
    float playerwidth = 35;
    float currPosRight = data->worldPos.x + playerwidth;
    for (int i = currPosRight-2; i < currPosRight; i++)
    {
        color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
            i, data->worldPos.y);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);
        if (!(R == 255 && G == 0 && B == 255))
        {
            if (data->isKnockBack)
            {
                data->knockBackPower = 0;
                data->isKnockBack = false;
                break;
            }
            if (R == 0 && G == 0 && B == 0)break;
            data->worldPos.x = i - playerwidth-2;
            data->rightWall = true;
            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
            data->rightWall = false;
    }
}
