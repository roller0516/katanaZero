#include "EnemyEffect.h"
#include "Camera.h"
#include "Image.h"
#include "Enemy.h"

HRESULT EnemyEffect::Init(int x, int y, EnemyEffectType type)
{
    ImageManager::GetSingleton()->GetSingleton()->AddImage("EnemySpark", "Image/Katana/effect/effect_gunspark_4x2.bmp", 392, 156, 4, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->GetSingleton()->AddImage("EnemySlash", "Image/Katana/enemy/enemy_grunt_slash_5x2.bmp", 600, 148, 5, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->GetSingleton()->AddImage("blood_ani", "Image/Katana/effect/effect_blood_7x2.bmp", 560, 158, 7, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->GetSingleton()->AddImage("blood1", "Image/Katana/effect/effect_blood_remain1_R.bmp", 118, 113,1,1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->GetSingleton()->AddImage("blood2", "Image/Katana/effect/effect_blood_remain2_R.bmp", 120, 116,1,1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->GetSingleton()->AddImage("blood3", "Image/Katana/effect/effect_blood_remain3_R.bmp", 128, 123,1,1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->GetSingleton()->AddImage("blood4", "Image/Katana/effect/effect_blood_remain4_R.bmp", 144, 144,1,1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->GetSingleton()->AddImage("blood1_flip", "Image/Katana/effect/effect_blood_remain1_L.bmp", 118, 113, 1, 1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->GetSingleton()->AddImage("blood2_flip", "Image/Katana/effect/effect_blood_remain2_L.bmp", 120, 116, 1, 1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->GetSingleton()->AddImage("blood3_flip", "Image/Katana/effect/effect_blood_remain3_L.bmp", 128, 123, 1, 1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->GetSingleton()->AddImage("blood4_flip", "Image/Katana/effect/effect_blood_remain4_L.bmp", 144, 144, 1, 1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->GetSingleton()->AddImage("Find", "Image/Katana/effect/enemy_follow.bmp", 8, 10, true, RGB(255, 0, 255));
    this->Type = type;
    
    worldPos.x = x;
    worldPos.y = y;
    isAlive = false;
    currFrame = 0;
    return S_OK;
}

void EnemyEffect::Release()
{

}

void EnemyEffect::Update()
{
    if (isAlive) 
    {
        if (Type == EnemyEffectType::blood_ani  || Type == EnemyEffectType::EnemySpark) 
        {
            currFrame += 10 * TimerManager::GetSingleton()->GetElapsedTime();
            if (currFrame > maxFrame) 
            {
                currFrame = 0;
                isAlive = false;
            }
        }
        if (Type == EnemyEffectType::Find) 
        {
            currFrame += 15 * TimerManager::GetSingleton()->GetElapsedTime();
            if (currFrame > 2)
            {
                currFrame = 0;
                isAlive = false;
            }
        }
        if (Type == EnemyEffectType::EnemySlash) 
        {
            currFrame += 15*TimerManager::GetSingleton()->GetElapsedTime();
            if (currFrame > maxFrame)
            {
                currFrame = 0;
                isAlive = false;
            }
        }
    }
    
    localPos.x = worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
    localPos.y = worldPos.y - Camera::GetSingleton()->GetCameraPos().y;
}

void EnemyEffect::Render(HDC hdc)
{
    if (isAlive) 
    {
        switch (Type)
        {
        case EnemyEffectType::EnemySpark:
            maxFrame = 4;
            img = ImageManager::GetSingleton()->FindImage("EnemySpark");
            if (dir == EnemyEffectDir::Right)
                img->rotateRender(hdc, localPos.x, localPos.y, currFrame, 0, owner->GetData()->attackAngle,0.5f);
            else
                img->rotateRenderFlip(hdc, localPos.x, localPos.y, currFrame, 0, owner->GetData()->attackAngle,0.5f);
            break;
        case EnemyEffectType::EnemySlash:
            maxFrame = 5;
            img = ImageManager::GetSingleton()->FindImage("EnemySlash");
            if (dir == EnemyEffectDir::Right)
                img->FrameRender(hdc, localPos.x, localPos.y, currFrame, 0, true);
            else
                img->FrameRenderFlip(hdc, localPos.x, localPos.y, currFrame, 0, true);
            break;
        case EnemyEffectType::blood_ani:
            maxFrame = 7;
            img = ImageManager::GetSingleton()->FindImage("blood_ani");
            if (dir == EnemyEffectDir::Right)
                img->FrameRender(hdc, localPos.x, localPos.y, currFrame, 0, true);
            else
                img->FrameRenderFlip(hdc, localPos.x, localPos.y, currFrame, 0, true);
            break;
        case EnemyEffectType::blood1:
            
            if (dir == EnemyEffectDir::Right)
                img = ImageManager::GetSingleton()->FindImage("blood1");
            else
                img = ImageManager::GetSingleton()->FindImage("blood1_flip");

            img->AlphaRender(hdc, localPos.x, localPos.y, 0, 0, 100, true);
            break;
        case EnemyEffectType::blood2:
            
            if (dir == EnemyEffectDir::Right)
                img = ImageManager::GetSingleton()->FindImage("blood2");
            else
                img = ImageManager::GetSingleton()->FindImage("blood2_flip");
            img->AlphaRender(hdc, localPos.x, localPos.y, 0, 0, 100, true);
            break;
        case EnemyEffectType::blood3:
            if (dir == EnemyEffectDir::Right)
                img = ImageManager::GetSingleton()->FindImage("blood3");
            else
                img = ImageManager::GetSingleton()->FindImage("blood3_flip");

            img->AlphaRender(hdc, localPos.x, localPos.y, 0, 0, 150, true);
            break;
        case EnemyEffectType::blood4:
            if (dir == EnemyEffectDir::Right)
                img = ImageManager::GetSingleton()->FindImage("blood4");
            else
                img = ImageManager::GetSingleton()->FindImage("blood4_flip");
            img->AlphaRender(hdc, localPos.x, localPos.y, 0, 0, 100, true);
            break;
        case EnemyEffectType::Find:
            img = ImageManager::GetSingleton()->FindImage("Find");
            img->Render(hdc, localPos.x, localPos.y, true);
            break;
        }

    }
}

