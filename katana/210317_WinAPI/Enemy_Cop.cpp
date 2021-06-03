#include "Enemy_Cop.h"
#include "Camera.h"
#include "Image.h"
#include "Player.h"
#include "CommonFunction.h"
#include "MissileManager.h"
#include "AstarManager.h"
#include "EnemyEffect.h"
HRESULT Enemy_Cop::Init(int posX, int posY)
{
    data = new Enemy::EnemyData;
    data->image = ImageManager::GetSingleton()->AddImage("Cop_Idle", "Image/Katana/enemy/enemy_cop_idle_8x2.bmp", 576, 148, 8, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Cop_hurt", "Image/Katana/enemy/enemy_cop_hurt_14x2.bmp", 1148, 136, 14, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Cop_run", "Image/Katana/enemy/enemy_cop_run_10x2.bmp", 900, 160, 10, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Cop_walk", "Image/Katana/enemy/enemy_cop_walk_8x2.bmp", 432, 152, 8, 2, true, RGB(255, 0, 255));
    armLImage = ImageManager::GetSingleton()->AddImage("Cop_arm_l", "Image/Katana/enemy/enemy_cop_leftarm_R.bmp", 28, 16, 1, 1, true, RGB(255, 0, 255));
    armRImage = ImageManager::GetSingleton()->AddImage("Cop_arm_r", "Image/Katana/enemy/enemy_cop_rightarm_R.bmp", 30, 18, 1, 1, true, RGB(255, 0, 255));
    GunImage = ImageManager::GetSingleton()->AddImage("Cop_gun", "Image/Katana/enemy/enemy_bold_gun.bmp", 54, 12, 1, 1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("Cop_attack", "Image/Katana/enemy/enemy_Cop_aim_R.bmp", 46, 72, 1, 1, true, RGB(255, 0, 255));
    data->astar = new AstarManager;
    data->astar->Init();
    enemyEffect = new EnemyEffect[8];

    for (int i = 0; i < 8; i++)
    {
        enemyEffect[i].Init(data->worldPos.x, data->worldPos.y, (EnemyEffectType)i);
    }

    data->isTurn = false;
    data->findRange = 200;
    data->attackRange = 300;
    data->worldPos.x = posX;
    data->worldPos.y = posY;
    data->maxFrame = 8;
    data->moveSpeed = 1000;
    data->size = data->image->GetImageInfo()->frameWidth;
    data->Name = "Cop";
    data->isAlive = true;
    data->attackAngle = 0;
    data->knockBackPower = 500;
    data->Index = 0;
    return S_OK;
}

void Enemy_Cop::Release()
{
   
}

void Enemy_Cop::Update()
{
    if (data->isRender)
    {
        for (int i = 0; i < 8; i++)
        {
            enemyEffect[i].Update();
        }
        if (data->astar)
            data->astar->Update();

        if (data->isalhpa)
        {
            data->alpahcount += 1;
            if (data->alpahcount > 255)
            {
                data->alpahcount = 255;
            }
        }

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
            data->attackAngle = GetAngle(this->data->worldPos, data->target->GetWorldpos());
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
    }
}

void Enemy_Cop::Render(HDC hdc, bool world)
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
                GunImage->FrameRenderFlip(hdc, data->worldPos.x - 20, data->worldPos.y - 12, 0, 0, true);
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
    else if (data->isRender)
    {
        
        if (data->localPos.x > WINSIZE_X || data->localPos.y > WINSIZE_Y)
            return;
        int y;
        if (isBlack)
            y = 10;
        else
            y = 0;
        if (!data->isalhpa || data->alpahcount == 255)
        {
            if (dir == EnemyDir::Left)
            {
                enemyEffect[2].SetPos(data->worldPos.x + 30, data->worldPos.y - 25);
                enemyEffect[2].SetDir(EnemyEffectDir::Right);
                enemyEffect[0].SetDir(EnemyEffectDir::Left);
                enemyEffect[0].SetPos(data->worldPos.x - 50, data->worldPos.y - 7);

                if (data->isAttack)
                {
                    armLImage->rotateRenderFlip(hdc, data->localPos.x - 20, data->localPos.y - 10 - y, 0, 0, data->attackAngle);
                    data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y - y, data->currFrameX, 0, true);
                    GunImage->rotateRenderFlip(hdc, data->localPos.x - 10, data->localPos.y - 10 - y, 0, 0, data->attackAngle);
                    armRImage->rotateRenderFlip(hdc, data->localPos.x + 5, data->localPos.y - 10 - y, 0, 0, data->attackAngle);
                }
                else if (state == EnemyState::hurt)
                    data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
                else
                    data->image->FrameRenderFlip(hdc, data->localPos.x, data->localPos.y - y, data->currFrameX, 0, true);
            }
            else
            {
                enemyEffect[2].SetPos(data->worldPos.x - 30, data->worldPos.y - 25);
                enemyEffect[2].SetDir(EnemyEffectDir::Left);
                enemyEffect[0].SetDir(EnemyEffectDir::Right);
                enemyEffect[0].SetPos(data->worldPos.x + 50, data->worldPos.y - 14);

                if (data->isAttack)
                {
                    armLImage->rotateRender(hdc, data->localPos.x + 20, data->localPos.y - y - 10, 0, 0, data->attackAngle);
                    data->image->FrameRender(hdc, data->localPos.x, data->localPos.y - y, data->currFrameX, 0, true);
                    GunImage->rotateRender(hdc, data->localPos.x + 10, data->localPos.y - 10 - y, 0, 0, data->attackAngle);
                    armRImage->rotateRender(hdc, data->localPos.x - 5, data->localPos.y - 10 - y, 0, 0, data->attackAngle);
                }
                else if (state == EnemyState::hurt)
                    data->image->FrameRender(hdc, data->localPos.x, data->localPos.y, data->currFrameX, 0, true);
                else
                    data->image->FrameRender(hdc, data->localPos.x, data->localPos.y - y, data->currFrameX, 0, true);
            }
        }
        else if (data->isalhpa)
        {
            data->image->AlphaRender(hdc, data->localPos.x, data->localPos.y - y, data->currFrameX, 0, data->alpahcount, true);
        }


    }

    for (int i = 0; i < 8; i++)
    {
        if (i == 2)
            continue;
        enemyEffect[i].Render(hdc);
    }
    enemyEffect[2].Render(hdc);

}

Enemy* Enemy_Cop::Clone()
{
    return new Enemy_Cop;
}

void Enemy_Cop::Pattern()
{
    if (data->isAlive == false)
    {
        data->isKnockBack = true;
        Die();
        return;
    }
    if (data->target && data->target->GetIsAlive() == false)
    {
        Idle();
        return;
    }
    if (data->target == nullptr) return;

    float distance = Distance(this->data->worldPos, data->target->GetWorldpos());

    if (data->attackRange > distance) // 찾았고 공격 거리에있을때
    {

        if (data->attackAngle < DegToRad(30) && data->attackAngle > DegToRad(-30))
        {
            data->isAttack = true;
            data->isFind = false;
            dir = EnemyDir::Right;
            Attack(dir);
            return;
        }
        else if (data->attackAngle >= DegToRad(150) && data->attackAngle <= DegToRad(180) ||
            data->attackAngle <= DegToRad(-150) && data->attackAngle >= DegToRad(-180))
        {
            data->isAttack = true;
            data->isFind = false;
            dir = EnemyDir::Left;
            Attack(dir);
            return;
        }
    }
    if (data->findRange > distance)
        data->isAttack = false;

    //찾았을때 무조건 내위치로 따라옴
    data->findCooltime += TimerManager::GetSingleton()->GetElapsedTime();
    if (data->isFind)
    {
        data->delay = 0;
        Run();
        return;
    }
    if (data->findRange > distance)
    {
        if (data->findCount == 0)
        {
            enemyEffect[7].SetAlive(true);
            enemyEffect[7].SetPos(data->worldPos.x, data->worldPos.y - 50);

            data->findCount++;
        }
        if (data->findCooltime > 0.5f)
        {
            data->findCooltime = 0;
            data->isFind = true;
            return;
        }
        data->isFind = false;
        //data->isFind = true;
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

void Enemy_Cop::Attack(EnemyDir dir)
{
    data->isTurn = false;
    data->astar->Clear();
    targeton = 0;
    data->attackSpeed += TimerManager::GetSingleton()->GetElapsedTime();
    if (data->attackSpeed > 0.5f)
    {
        enemyEffect[0].SetOwner(this);
        data->attackSpeed = 0;
        if (dir == EnemyDir::Left)
        {
            data->missileManager->Fire(data->attackAngle, data->worldPos.x - 5, data->worldPos.y - 10,0);
            enemyEffect[0].SetAlive(true);
        }

        else
        {
            data->missileManager->Fire(data->attackAngle, data->worldPos.x + 5, data->worldPos.y - 10,0);
            enemyEffect[0].SetAlive(true);
        }

    }
    state = EnemyState::attack;
    Animation(state);
}

void Enemy_Cop::Run()
{
    if (data->isSamPle)
        data->moveSpeed = 0;
    else
        data->moveSpeed = 500;


    if (data->astar->GetBackTile())
    {
        int bx = data->astar->GetBackTile()->GetIdX();
        int by = data->astar->GetBackTile()->GetIdY();

        int cx = data->astar->GetTileIndex().x;
        int cy = data->astar->GetTileIndex().y;


        currPos.x = data->astar->GetMap(cx, cy)->GetCenter().x;
        currPos.y = data->astar->GetMap(cx, cy)->GetCenter().y;

        data->astar->GetBackTile()->SetColor(RGB(100, 100, 100), false);

        if (bx == cx && by == cy)
        {
            if (data->astar->GetParentList().size() == 0)
            {
                data->isTurn = false;
                data->astar->SetBackTile(data->astar->GetDestTile());
                data->astar->Clear();
                data->moveSpeed = 0;
                targeton = 0;
            }
            else if (data->astar->GetParentList().size() > 0)
            {
                data->astar->SetBackTile(data->astar->GetParentList().back());
                data->astar->ParentPopBack(data->Index);
            }
        }

        float centerX = data->astar->GetBackTile()->GetCenter().x;
        float centerY = data->astar->GetBackTile()->GetCenter().y;

        float angle = GetAngle(currPos, data->astar->GetBackTile()->GetCenter());

        data->worldPos.x += cosf(angle) * data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
        data->worldPos.y -= sinf(angle) * data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();

        if (centerX >= currPos.x)
            dir = EnemyDir::Right;
        else
            dir = EnemyDir::Left;

    }
    else if (data->isalhpa)
    {
        float angle = GetAngle(data->worldPos, data->target->Getpos());

        data->worldPos.x += cosf(angle) * data->moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();

        if (data->target->Getpos().x >= data->worldPos.x)
            dir = EnemyDir::Right;
        else
            dir = EnemyDir::Left;
    }

    state = EnemyState::run;
    Animation(state);
}

void Enemy_Cop::Walk()
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

void Enemy_Cop::Hurt()
{
    data->moveSpeed = 0;
    state = EnemyState::hurt;
    Animation(state);
}

void Enemy_Cop::Idle()
{
    data->moveSpeed = 0;
    state = EnemyState::idle;
    Animation(state);
}

void Enemy_Cop::Die()
{
    if (data->isAlive == false)
    {
        if (count == 0)
        {
            enemyEffect[2].SetAlive(true);
            srand((unsigned)time(NULL));
            int num = (rand() % 4) + 3;

            enemyEffect[num].SetAlive(true);
            enemyEffect[num].SetPos(data->worldPos.x - 50, data->worldPos.y - 50);
            if (dir == EnemyDir::Left)
                enemyEffect[num].SetDir(EnemyEffectDir::Right);
            else
                enemyEffect[num].SetDir(EnemyEffectDir::Left);
            count++;
        }

        data->shape = { -100,-100,-100,-100 };
        data->isFind = false;
        data->isAttack = false;
        state = EnemyState::hurt;
        Animation(state);
    }
}

void Enemy_Cop::KnockBack()
{
    float angle = data->target->GetPlayerAngle();
    if (data->target->GetDoor() == true)
        angle = 0;
    if (data->knockBackPower <= 0)
    {
        data->knockBackPower = 0;
        data->isKnockBack = false;
        return;
    }
    data->fallForce = 0;
    data->knockBackPower -= 500 * TimerManager::GetSingleton()->GetElapsedTime();
    data->worldPos.x += cosf(angle) * data->knockBackPower * TimerManager::GetSingleton()->GetElapsedTime();
    data->worldPos.y -= sinf(angle) * 1000 * TimerManager::GetSingleton()->GetElapsedTime();
}


void Enemy_Cop::Animation(EnemyState ani)
{
    switch (ani)
    {
    case EnemyState::idle:
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("Cop_Idle");
        break;
    case EnemyState::walk:
        data->maxFrame = 8;
        data->image = ImageManager::GetSingleton()->FindImage("Cop_walk");
        break;
    case EnemyState::run:
        data->maxFrame = 10;
        data->image = ImageManager::GetSingleton()->FindImage("Cop_run");
        break;
    case EnemyState::attack:
        data->maxFrame = 1;
        data->image = ImageManager::GetSingleton()->FindImage("Cop_attack");
        break;
    case EnemyState::hurt:
        data->maxFrame = 14;
        data->image = ImageManager::GetSingleton()->FindImage("Cop_hurt");
        break;
    }
}
void Enemy_Cop::PixelCollisionBottom()
{
    COLORREF color;
    int R, G, B;
    float playerHeight = 34;
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

            if (data->isAlive && (R == 0 && G == 0 && B == 0))
            {
                isBlack = true;
                //data->isTurn = true;
                data->isPhysic = false;
                break;
            }
            isBlack = false;
            data->velocity = 100;
            data->fallForce = 0;
            data->isPhysic = true;

            if (!data->isAlive)
            {
                data->worldPos.y = i - playerHeight;
                break;
            }

            data->worldPos.y = i - playerHeight - 2;
            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
        {
            data->velocity = 100;
            data->isTurn = false;
            data->isPhysic = true;
        }
    }
}
void Enemy_Cop::PixelCollisionLeft()
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
            data->worldPos.x = i + playerwidth + 5;
            data->leftWall = true;
            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
            data->leftWall = false;
    }
}

void Enemy_Cop::PixelCollisionRight()
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
            if (data->isKnockBack)
            {
                data->knockBackPower = 0;
                data->isKnockBack = false;
                break;
            }
            if (R == 0 && G == 0 && B == 0)break;
            data->worldPos.x = i - playerwidth - 2;
            data->rightWall = true;
            break;
        }
        else if ((R == 255 && G == 0 && B == 255))
            data->rightWall = false;
    }
}

void Enemy_Cop::MaptoolAstar()
{
    Run();
}
