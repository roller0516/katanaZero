#include "Enemy_Bold.h"
#include "Camera.h"
#include "Image.h"
#include "Player.h"
#include "CommonFunction.h"
HRESULT Enemy_Bold::Init(int posX, int posY)
{
	image = ImageManager::GetSingleton()->AddImage("Bold_Idle", "Image/Katana/enemy/enemy_bold_Idle_8x2.bmp", 576, 140, 8, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Bold_hurt", "Image/Katana/enemy/enemy_bold_hurt_14x2.bmp", 1036, 128, 14, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Bold_run", "Image/Katana/enemy/enemy_bold_run_10x2.bmp", 900, 156, 10, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Bold_walk", "Image/Katana/enemy/enemy_bold_walk_8x2.bmp", 544, 152, 8, 2, true, RGB(255, 0, 255));
    armLImage = ImageManager::GetSingleton()->AddImage("Bold_arm_l", "Image/Katana/enemy/enemy_bold_leftarm_R.bmp", 28, 16, 1, 1, true, RGB(255, 0, 255));
    armRImage = ImageManager::GetSingleton()->AddImage("Bold_arm_r", "Image/Katana/enemy/enemy_bold_rightarm_R.bmp", 30, 18, 1, 1, true, RGB(255, 0, 255));
    GunImage = ImageManager::GetSingleton()->AddImage("Bold_gun", "Image/Katana/enemy/enemy_bold_gun.bmp", 54, 12, 1, 1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Bold_attack", "Image/Katana/enemy/enemy_bold_aim_R.bmp", 42, 70, 1, 1, true, RGB(255, 0, 255));
    data = new Enemy::EnemyData;
    data->range = 200;
    worldPos.x = posX;
    worldPos.y = posY;
    data->maxFrame = 8;
    data->moveSpeed = 200;
    target = nullptr;
	return S_OK;
}

void Enemy_Bold::Release()
{
}

void Enemy_Bold::Update()
{
    
    localPos.x = worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
    localPos.y = worldPos.y - Camera::GetSingleton()->GetCameraPos().y;
    if (data->isAlive)
    {
        data->currFrameX += TimerManager::GetSingleton()->GetElapsedTime() * 10;
        if (data->currFrameX >= data->maxFrame)
            data->currFrameX = 0;
    }
    if (data->isPhysic&&data->isSamPle ==false)
    {
        data->fallForce -= Gravity * TimerManager::GetSingleton()->GetElapsedTime() * data->velocity/* * TimerManager::GetSingleton()->GetElapsedTime()*/;
        worldPos.y -= data->fallForce * TimerManager::GetSingleton()->GetElapsedTime();
    }
    if (data->isSamPle == false) 
    {
        Pattern();
        PixelCollisionBottom();
        PixelCollisionRight();
        PixelCollisionLeft();
    }
    
    if (KeyManager::GetSingleton()->IsOnceKeyDown('1'))
        Run();
    if (KeyManager::GetSingleton()->IsOnceKeyDown('2'))
        Walk();
    if (KeyManager::GetSingleton()->IsOnceKeyDown('3'))
        Hurt();
    if (KeyManager::GetSingleton()->IsOnceKeyDown('4')) 
        Attack();
}

void Enemy_Bold::Render(HDC hdc, bool world)
{
    if (data->isAlive)
    {
        if (world)
        {
            if ((dir == Enemy::EnemyDir::Left)) 
            {
                if (data->isAttack)
                {
                    armLImage->FrameRenderFlip(hdc, worldPos.x - 20, worldPos.y - 10, 0, 0, true);
                    image->FrameRenderFlip(hdc, worldPos.x, worldPos.y, data->currFrameX, 0, true);
                    GunImage->FrameRenderFlip(hdc, worldPos.x - 20, worldPos.y -12, 0, 0, true);
                    armRImage->FrameRenderFlip(hdc, worldPos.x, worldPos.y - 10, 0, 0, true);
                }
                else
                    image->FrameRenderFlip(hdc, worldPos.x, worldPos.y, data->currFrameX, 0, true);
            } 
            else 
            {
                if (data->isAttack)
                {
                    armLImage->FrameRender(hdc, worldPos.x + 20, worldPos.y - 10, 0, 0, true);
                    image->FrameRender(hdc, worldPos.x, worldPos.y, data->currFrameX, 0, true);
                    GunImage->FrameRender(hdc, worldPos.x + 20, worldPos.y - 12, 0, 0, true);
                    armRImage->FrameRender(hdc, worldPos.x, worldPos.y - 10, 0, 0, true);
                }
                else
                    image->FrameRender(hdc, worldPos.x, worldPos.y, data->currFrameX, 0, true);
            }
                
        }
        else 
        {
            if (dir == Enemy::EnemyDir::Left) 
            {
                if (data->isAttack)
                {
                    armLImage->FrameRenderFlip(hdc, localPos.x - 20, localPos.y - 10, 0, 0, true);
                    image->FrameRenderFlip(hdc, localPos.x, localPos.y, data->currFrameX, 0, true);
                    GunImage->FrameRenderFlip(hdc, localPos.x - 20, localPos.y - 12, 0, 0, true);
                    armRImage->FrameRenderFlip(hdc, localPos.x, localPos.y, 0, 0, true);
                }
                else
                    image->FrameRenderFlip(hdc, localPos.x, localPos.y, data->currFrameX, 0, true);
            }  
            else 
            {
                if (data->isAttack)
                {
                    armLImage->FrameRender(hdc, localPos.x + 20, localPos.y - 10, 0, 0, true);
                    image->FrameRender(hdc, localPos.x, localPos.y, data->currFrameX, 0, true);
                    GunImage->FrameRender(hdc, localPos.x + 20, localPos.y - 12, 0, 0, true);
                    armRImage->FrameRender(hdc, localPos.x, localPos.y - 10, 0, 0, true);
                }
                else
                    image->FrameRender(hdc, localPos.x, localPos.y, data->currFrameX, 0, true);
            }    
        }
    }
}

Enemy* Enemy_Bold::Clone()
{
	return new Enemy_Bold;
}

void Enemy_Bold::Pattern()
{
    if (data->range > Distacne(this->worldPos, target->GetWorldpos()))
    {
        data->isFind = true;
        Run();
        return;
    }

    if (data->leftWall)
        dir = Enemy::EnemyDir::Right;
    if (data->rightWall)
        dir = Enemy::EnemyDir::Left;

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
    }
}

void Enemy_Bold::Attack()
{
    if (target)
    {
        if (data->range > Distacne(this->worldPos, target->GetWorldpos())) 
        {
            data->isAttack = true;
            state = Enemy::EnemyState::attack;
            Animation(state);
        }    
    }
}

void Enemy_Bold::Run()
{
    state = Enemy::EnemyState::run;
    Animation(state);

    if (data->isSamPle)
        data->moveSpeed = 0;
    else
        data->moveSpeed = 150;

    if (data->isFind) // Ã£À½ 
    {
        float angle;
        angle = GetAngle(target->GetWorldpos(), this->worldPos);
        if (angle <= PI / 2 && angle >= -PI / 2) 
        {
            worldPos.x -= cosf(angle) * data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
            worldPos.y -= cosf(angle) * data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
            dir = Enemy::EnemyDir::Left;
        }
        else
        {
            worldPos.x -= cosf(angle) * data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
            worldPos.y -= cosf(angle) * data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
            dir = Enemy::EnemyDir::Right;
        }
        return;
    }

    if (dir == Enemy::EnemyDir::Right)
        worldPos.x += data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    else
        worldPos.x -= data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();

}

void Enemy_Bold::Walk()
{
    if (data->isSamPle)
        data->moveSpeed = 0;
    else
        data->moveSpeed = 100;

    if(dir == Enemy::EnemyDir::Right)
        worldPos.x += data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    else
        worldPos.x -= data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    state = Enemy::EnemyState::walk;
    Animation(state);
}

void Enemy_Bold::Hurt()
{
    data->moveSpeed = 0;
    state = Enemy::EnemyState::hurt;
    Animation(state);
}

void Enemy_Bold::Idle()
{
    data->moveSpeed = 0;
    state = Enemy::EnemyState::idle;
    Animation(state);
}


void Enemy_Bold::Animation(Enemy::EnemyState ani)
{
    switch (ani)
    {
    case Enemy::idle:
        data->maxFrame = 8;
        image = ImageManager::GetSingleton()->FindImage("Bold_Idle");
        break;
    case Enemy::walk:
        data->maxFrame = 8;
        image = ImageManager::GetSingleton()->FindImage("Bold_walk");
        break;
    case Enemy::run:
        data->maxFrame = 10;
        image = ImageManager::GetSingleton()->FindImage("Bold_run");
        break;
    case Enemy::attack:
        data->maxFrame = 1;
        image = ImageManager::GetSingleton()->FindImage("Bold_attack");
        break;
    case Enemy::hurt:
        data->maxFrame = 14;
        image = ImageManager::GetSingleton()->FindImage("Bold_hurt");
        break;
    }
}
void Enemy_Bold::PixelCollisionBottom()
{
    COLORREF color;
    int R, G, B;
    float playerHeight = 36;
    float currPosBottom = worldPos.y + playerHeight;
    for (int i = currPosBottom - 5; i < currPosBottom + 5; i++)
    {
        color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
            worldPos.x, i);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);

        if (!(R == 255 && G == 0 && B == 255))
        {
            data->velocity = 60;
            worldPos.y = i - playerHeight;
            data->fallForce = 0;
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
    float currPosLeft = worldPos.x - playerwidth;
    for (int i = currPosLeft; i < currPosLeft + 2; i++)
    {
        color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
            i, worldPos.y);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);
        if (!(R == 255 && G == 0 && B == 255))
        {
            if (R == 0 && G == 0 && B == 0)break;
            worldPos.x = i + playerwidth+5;
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
    float currPosRight = worldPos.x + playerwidth;
    for (int i = currPosRight-2; i < currPosRight; i++)
    {
        color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
            i, worldPos.y);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);
        if (!(R == 255 && G == 0 && B == 255))
        {
            if (R == 0 && G == 0 && B == 0)break;
            worldPos.x = i - playerwidth-2;
            data->rightWall = true;
            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
            data->rightWall = false;
    }
}
