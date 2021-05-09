#include "Enemy.h"
#include "CommonFunction.h"
#include "Image.h"
#include "MissileManager.h"
#include "Player.h"

HRESULT Enemy::Init(int posX, int posY)
{
    image = ImageManager::GetSingleton()->FindImage("Enemy");
    if (image == nullptr)
    {
        MessageBox(g_hWnd, 
            "Enemy에 해당하는 이미지가 추가되지 않았음!", "경고", MB_OK);
        return E_FAIL;
    }

    currFrameX = 0;
    updateCount = 0;

    pos.x =  posX;
    pos.y = posY;
    size = 80;
    name = "NormalEnemy";
    shape = { 0, 0, 0, 0 };
    moveSpeed = 3.3f;
    isAlive = true;
    angle = 0.0f;
    target = nullptr;
    dir = 1;

    // 미사일 매니저
    missileMgr = new MissileManager();
    missileMgr->Init(this);

    fireCount = 0;

    return S_OK;
}

void Enemy::Release()
{
    SAFE_RELEASE(missileMgr);
}

void Enemy::Update()
{
    if (isAlive)
    {
        //HorizonMove();
        //Move();

        // 미사일 발사
        if (missileMgr)
        {
            // 함수 호출 주기를 바꿔보자.
            fireCount++;
            if (fireCount % 20 == 0)
            {
                fireCount = 0;
                missileMgr->Fire();
            }
            missileMgr->Update();
        }

        // 애니메이션 프레임
        updateCount++;
        if (updateCount == 5)
        {
            currFrameX++;
            if (currFrameX >= 10)
            {
                currFrameX = 0;
            }

            updateCount = 0;
        }

    }
}

void Enemy::Render(HDC hdc)
{
    if (isAlive)
    {
        RenderEllipseToCenter(hdc, pos.x, pos.y, size, size);

        if (image)
        {
            image->FrameRender(hdc, pos.x, pos.y, currFrameX, 0, true);
        }

        if (missileMgr)
        {
            missileMgr->Render(hdc);
        }
    }
}

void Enemy::Move()
{
    //if (target)
    //{
    //    FPOINT targetPos = target->GetPos();

    //    // 현재 위치에서 타겟 위치로 이동할 수 있는 각도 구하기
    //    float x = targetPos.x - pos.x;
    //    float y = targetPos.y - pos.y;

    //    angle = atan2(y, x);

    //    pos.x += cosf(angle) * moveSpeed;
    //    pos.y += sinf(angle) * moveSpeed;
    //}
}

void Enemy::HorizonMove()
{
    if (pos.x > WINSIZE_X || pos.x < 0)
    {
        dir *= -1;
    }
    pos.x += moveSpeed * dir;
}
