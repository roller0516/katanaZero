#include "Enemy_Grunt.h"
#include "Camera.h"
#include "Image.h"
#include "Player.h"
#include "CommonFunction.h"
#include "MissileManager.h"
#include "AstarManager.h"
HRESULT Enemy_Grunt::Init(int posX, int posY)
{
    data = new Enemy::EnemyData;
    data->image = ImageManager::GetSingleton()->AddImage("Grunt_Idle", "Image/Katana/enemy/enemy_grunt_idle_8x2.bmp", 480, 144, 8, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Grunt_hurt", "Image/Katana/enemy/enemy_grunt_hurt_15x2.bmp", 1680, 164, 15, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Grunt_run", "Image/Katana/enemy/enemy_grunt_run_10x2.bmp", 720, 156, 10, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Grunt_walk", "Image/Katana/enemy/enemy_grunt_walk_10x2.bmp", 640, 160, 10, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Grunt_attack", "Image/Katana/enemy/enemy_grunt_attack_7x2.bmp", 616, 168, 7, 2, true, RGB(255, 0, 255));
    
    
    //missileManager = new MissileManager();
    //missileManager->Init(this);
    //data->astar = new AstarManager();
    //data->astar->Init();
    //data->astar->SetOnwer(this);

    data->isTurn = false;
    data->findRange = 200;
    data->attackRange = 70;
    data->worldPos.x = posX;
    data->worldPos.y = posY;
    data->maxFrame = 8;
    data->moveSpeed = 200;
    data->size = data->image->GetImageInfo()->frameWidth;
    data->Name = "Grunt";
    data->isAlive = true;
    data->attackAngle = 0;
    data->knockBackPower = 800;
    data->Index = 0;
    return S_OK;
}

void Enemy_Grunt::Release()
{
    
}

void Enemy_Grunt::Update()
{
    if (data->astar)
        data->astar->Update();

    data->localPos.x = data->worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
    data->localPos.y = data->worldPos.y - Camera::GetSingleton()->GetCameraPos().y;

    data->currFrameX += TimerManager::GetSingleton()->GetElapsedTime() * 10;
    if (data->currFrameX >= data->maxFrame)
    {
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
    if (data->isSamPle == false && data->target == nullptr)
        MaptoolAstar();

    if (data->isSamPle == false)
    {
        Pattern();
        if (data->isKnockBack)
            KnockBack();
        PixelCollisionBottom();
        PixelCollisionRight();
        PixelCollisionLeft();
    }

    if (data->isPhysic && data->isSamPle == false)
    {
        data->fallForce -= Gravity * TimerManager::GetSingleton()->GetElapsedTime() * data->velocity/* * TimerManager::GetSingleton()->GetElapsedTime()*/;
        data->worldPos.y -= data->fallForce * TimerManager::GetSingleton()->GetElapsedTime();
    }
    if(data->isAttack == false)
        data->attackShape = { -100,-100,-100,-100 };
}

void Enemy_Grunt::Render(HDC hdc, bool world)
{
    //data->astar->Render(hdc);

    if (world)
    {
        if ((dir == EnemyDir::Left))
        {
            if (data->isAttack)
            {
                data->image->FrameRenderFlip(hdc, data->worldPos.x, data->worldPos.y, data->currFrameX, 0, true);
            }
            else
                data->image->FrameRenderFlip(hdc, data->worldPos.x, data->worldPos.y, data->currFrameX, 0, true);
        }
        else
        {
            if (data->isAttack)
            {
                data->image->FrameRender(hdc, data->worldPos.x, data->worldPos.y, data->currFrameX, 0, true);
            }
            else
                data->image->FrameRender(hdc, data->worldPos.x, data->worldPos.y, data->currFrameX, 0, true);
        }
    }
    else
    {
        if (data->localPos.x > WINSIZE_X || data->localPos.y > WINSIZE_Y)
            return;
        if (dir == EnemyDir::Left)
        {
            if (state == EnemyState::hurt)
                data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
            else
                data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
        }
        else
        {
           if (state == EnemyState::hurt)
                data->image->FrameRender(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
            else
                data->image->FrameRender(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
        }
        Rectangle(hdc, data->attackShape.left - Camera::GetSingleton()->GetCameraPos().x, data->attackShape.top - Camera::GetSingleton()->GetCameraPos().y,
            data->attackShape.right - Camera::GetSingleton()->GetCameraPos().x, data->attackShape.bottom - Camera::GetSingleton()->GetCameraPos().y);
    }
}

Enemy* Enemy_Grunt::Clone()
{
    return new Enemy_Grunt;
}

void Enemy_Grunt::Pattern()
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
        if (data->attackAngle < DegToRad(90) && data->attackAngle > DegToRad(-90))
        {
            data->isAttack = true;
            dir = EnemyDir::Right;
            Attack(dir);
            return;
        }
        else if (data->attackAngle >= DegToRad(90) && data->attackAngle <= DegToRad(180) ||
            data->attackAngle <= DegToRad(-90) && data->attackAngle >= DegToRad(-180))
        {
            data->isAttack = true;
            dir = EnemyDir::Left;
            Attack(dir);
            return;
        }
    }
    data->isAttack = false;
    //찾았을때 무조건 내위치로 따라옴
    data->findCooltime += TimerManager::GetSingleton()->GetElapsedTime();
   
    if (data->isFind)
    {
        data->delay = 0;
        Run();
        return;
    }
    if (data->findRange > distance )
    {
       if (data->findCooltime > 1.0f)
       {
           data->findCooltime = 0;
           data->isFind = true;
           return;
       }
       data->isFind = false;
    }
    
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

void Enemy_Grunt::Attack(EnemyDir dir)
{
    data->isTurn = false;
    data->astar->Clear();
    targeton = 0;
    data->attackSpeed += TimerManager::GetSingleton()->GetElapsedTime();
    
    if (data->attackSpeed > 0.2f)
    {
        data->attackSpeed = 0;
        // 어택 박스 생성

        if (dir == EnemyDir::Right) 
        {
            data->attackShape.left = data->worldPos.x + 40 - (40 / 2);
            data->attackShape.top = data->worldPos.y - 80 / 2;
            data->attackShape.right = data->worldPos.x + 40 + (40 / 2);
            data->attackShape.bottom = data->worldPos.y + 80 / 2;

        }
        else 
        {
            data->attackShape.left = data->worldPos.x - 40 - (40 / 2);
            data->attackShape.top = data->worldPos.y - 80 / 2;
            data->attackShape.right = data->worldPos.x - 40 + (40 / 2);
            data->attackShape.bottom = data->worldPos.y + 80 / 2;

        }
        state = EnemyState::attack;
        Animation(state);
    }
}

void Enemy_Grunt::Run()
{
    if (data->isSamPle)
        data->moveSpeed = 0;
    else
        data->moveSpeed = 150;

    if (data->astar->GetParentList(data->Index))
    {
        if (data->astar->GetBackTile())
        {
            int bx = data->astar->GetBackTile()->GetIdX();
            int by = data->astar->GetBackTile()->GetIdY();

            int cx = data->astar->GetTileIndex().x;
            int cy = data->astar->GetTileIndex().y;

            if (bx == cx && by == cy)
            {
                int x = data->astar->GetTileIndex().x;
                int y = data->astar->GetTileIndex().y;

                currPos.x = data->astar->GetMap(x, y)->GetCenter().x;
                currPos.y = data->astar->GetMap(x, y)->GetCenter().y + TILESIZE / 2;

                if (data->astar->GetParentList(data->Index)->size() == 0)
                {
                    data->isTurn = false;
                    data->astar->SetBackTile(data->astar->GetDestTile());
                    data->astar->Clear();
                    targeton = 0;
                }
                else
                {
                    data->astar->ParentPopBack(data->Index);
                    if (data->astar->GetParentList(data->Index)->size() > 1)
                    {
                        if (data->astar->GetParentList(data->Index)->size() > 0)
                            data->astar->SetBackTile(data->astar->GetParentList(data->Index)->back());
                        //data->astar->ParentPopBack(data->Index);
                    }
                    
                }
            }
            float centerX = data->astar->GetBackTile()->GetCenter().x;
            float centerY = data->astar->GetBackTile()->GetCenter().y + TILESIZE / 2;
            float x = abs(centerX - currPos.x);
            float y = abs(centerY - currPos.y);

            if (centerX >= currPos.x && centerY >= currPos.y)//오른쪽 아래
            {
                data->worldPos.x += x * 1 * TimerManager::GetSingleton()->GetElapsedTime();
                data->worldPos.y += y * 1 * TimerManager::GetSingleton()->GetElapsedTime();
            }

            else if (centerX > currPos.x && centerY < currPos.y)//왼쪽 아래
            {
                data->worldPos.x += x * 1 * TimerManager::GetSingleton()->GetElapsedTime();
                data->worldPos.y -= y * 1 * TimerManager::GetSingleton()->GetElapsedTime();
            }
            else if (centerX <= currPos.x && centerY >= currPos.y)//왼 위
            {
                data->worldPos.x -= x * 1 * TimerManager::GetSingleton()->GetElapsedTime();
                data->worldPos.y += y * 1 * TimerManager::GetSingleton()->GetElapsedTime();
            }
            else if (centerX < currPos.x && centerY < currPos.y) // rigt up
            {
                data->worldPos.x -= x * 1 * TimerManager::GetSingleton()->GetElapsedTime();
                data->worldPos.y -= y * 1 * TimerManager::GetSingleton()->GetElapsedTime();
            }

            if (centerX > currPos.x)
                dir = EnemyDir::Right;
            else
                dir = EnemyDir::Left;
        }


    }
    state = EnemyState::run;
    Animation(state);
}

void Enemy_Grunt::Walk()
{
    if (data->leftWall)
        dir = EnemyDir::Right;
    else if (data->rightWall)
        dir = EnemyDir::Left;

    if (data->isSamPle)
        data->moveSpeed = 0;
    else
        data->moveSpeed = 100;

    if (dir == EnemyDir::Right)
        data->worldPos.x += data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    else
        data->worldPos.x -= data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    state = EnemyState::walk;
    Animation(state);
}

void Enemy_Grunt::Hurt()
{
    data->moveSpeed = 0;
    state = EnemyState::hurt;
    Animation(state);
}

void Enemy_Grunt::Idle()
{
    data->moveSpeed = 0;
    state = EnemyState::idle;
    Animation(state);
}

void Enemy_Grunt::Die()
{
    if (data->isAlive == false)
    {
        data->shape = { -100,-100,-100,-100 };
        data->isFind = false;
        data->isAttack = false;
        state = EnemyState::hurt;
        Animation(state);
    }
}

void Enemy_Grunt::KnockBack()
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


void Enemy_Grunt::Animation(EnemyState ani)
{
    switch (ani)
    {
    case EnemyState::idle:
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("Grunt_Idle");
        break;
    case EnemyState::walk:
        data->maxFrame = 10;
        data->image = ImageManager::GetSingleton()->FindImage("Grunt_walk");
        break;
    case EnemyState::run:
        data->maxFrame = 10;
        data->image = ImageManager::GetSingleton()->FindImage("Grunt_run");
        break;
    case EnemyState::attack:
        data->maxFrame = 7;
        data->image = ImageManager::GetSingleton()->FindImage("Grunt_attack");
        break;
    case EnemyState::hurt:
        data->maxFrame = 15;
        data->image = ImageManager::GetSingleton()->FindImage("Grunt_hurt");
        break;
    }
}
void Enemy_Grunt::PixelCollisionBottom()
{
    if (data->isTurn) 
    {
        data->fallForce = 0;
        return;
    }
        
    COLORREF color;
    int R, G, B;
    float playerHeight = 28;
    float currPosBottom = data->worldPos.y + playerHeight;
    for (int i = currPosBottom-1 ; i < currPosBottom; i++)
    {
        color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
            data->worldPos.x, i);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);

        if (!(R == 255 && G == 0 && B == 255))
        {
            if (data->isAlive && (R == 0 && G == 0 && B == 0))
            {
                data->isTurn = true;
                data->isPhysic = false;
                break;
            }
            data->fallForce = 0;
            data->isTurn = false;
            data->isPhysic = true;
            if (!data->isAlive)
            {
                data->velocity = 80;
                data->worldPos.y = i - playerHeight;
                break;
            }
            //if (data->isTurn)
            //    break;
            data->worldPos.y = i - playerHeight;

            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
        {
            //data->isTurn = false;
            //data->isPhysic = true;
        }
    }
}
void Enemy_Grunt::PixelCollisionLeft()
{
    COLORREF color;
    int R, G, B;
    float playerwidth = 30;
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
            data->worldPos.x = i + playerwidth + 5;
            data->leftWall = true;
            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
            data->leftWall = false;
    }
}

void Enemy_Grunt::PixelCollisionRight()
{
    COLORREF color;
    int R, G, B;
    float playerwidth = 30;
    float currPosRight = data->worldPos.x + playerwidth;
    for (int i = currPosRight - 5; i < currPosRight; i++)
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
            data->worldPos.x = i - playerwidth;
            data->rightWall = true;
            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
            data->rightWall = false;
    }
}

void Enemy_Grunt::MaptoolAstar()
{
    Run();
}
