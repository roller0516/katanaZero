#include "Enemy_pomp.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Player.h"

HRESULT Enemy_pomp::Init(int posX, int posY)
{
    this->type = EnemyType::Pomp;
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

    fireCount = 0;

    return S_OK;
}

void Enemy_pomp::Release()
{
}

void Enemy_pomp::Update()
{
    if (isAlive)
    {
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

void Enemy_pomp::Render(HDC hdc)
{
    if (isAlive)
    {
        RenderEllipseToCenter(hdc, pos.x, pos.y, size, size);

        if (image)
        {
            image->FrameRender(hdc, pos.x, pos.y, currFrameX, 0, true);
        }
    }
}

Enemy* Enemy_pomp::Clone()
{
    return new Enemy_pomp();
}

void Enemy_pomp::Move()
{
   
}

void Enemy_pomp::HorizonMove()
{
    if (pos.x > WINSIZE_X || pos.x < 0)
    {
        dir *= -1;
    }
    pos.x += moveSpeed * dir;
}
