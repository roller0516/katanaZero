#include "PlayerEffect.h"
#include "Player.h"
#include "Image.h"
#include "Camera.h"
HRESULT PlayerEffect::Init(EffectType type, Player* player)
{
    ImageManager::GetSingleton()->AddImage("attackEffect", "Image/Katana/player/player_slash_basic_6x2.bmp",1260,120,6, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("jumpEffect", "Image/Katana/player/player_jumpdust_5x1.bmp",210,80,5, 1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("flipEffect", "Image/Katana/player/player_jumpdust_wall_2x5.bmp",160,210,2, 5, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("dustEffect", "Image/Katana/player/player_dust_7x1.bmp",238,28,7, 1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("landEffect", "Image/Katana/player/player_landdust_8x1.bmp",660 , 16, 8, 1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("reflectEffect", "Image/Katana/effect/effect_reflect_6x2.bmp", 883, 280, 6, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("hitEffect", "Image/Katana/effect/effect_hit_lazer.bmp", 900, 4,1,1,true, RGB(255, 0, 255));

    effectType = type;

    switch (effectType)
    {
    case EffectType::attackEffect:
        isFollow = true;
        maxFrame = 6;
        break;
    case EffectType::jumpEffect:
        maxFrame = 5;
        break;
    case EffectType::flipEffect:
        maxFrame = 5;
        break;
    case EffectType::dustEffect:
        maxFrame = 7;
        break;
    case EffectType::landEffect:
        maxFrame = 8;
        break;
    case EffectType::reflectEffect:
        maxFrame = 6;
    }
    isAlive = false;
    this->owner = player;
    worldPos = player->GetWorldpos();
    currFrame = 0;
    size = 1;
    moveSpeed = 0;
    return S_OK;
}

void PlayerEffect::Release()
{

}

void PlayerEffect::Update()
{
    if (isAlive) 
    {
        if (effectType == EffectType::hitEffect) 
        {
            worldPos.x += cosf(owner->GetPlayerAngle()) * 1000000 * TimerManager::GetSingleton()->GetElapsedTime();
            worldPos.y -= sinf(owner->GetPlayerAngle()) * 1000000 * TimerManager::GetSingleton()->GetElapsedTime();
        }
        else 
        {
            moveSpeed += TimerManager::GetSingleton()->GetElapsedTime() * 30;
            currFrame += 15 * TimerManager::GetSingleton()->GetElapsedTime();
            if (currFrame > maxFrame)
            {
                moveSpeed = 0;
                currFrame = 0;
                count = 0;
                isAlive = false;
                worldPos.x = -100;
                worldPos.y = -100;
            }
        }
    }

    localPos.x = worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
    localPos.y = worldPos.y - Camera::GetSingleton()->GetCameraPos().y;
}

void PlayerEffect::Render(HDC hdc)
{
    int dir = (int)owner->GetDir();

    if (isAlive) 
    {
        switch (effectType)
        {
        case EffectType::attackEffect:
            worldPos = owner->GetWorldpos();
            localPos = owner->Getpos();
            img = ImageManager::GetSingleton()->FindImage("attackEffect");
            img->rotateRender(hdc, localPos.x, localPos.y,currFrame,0, owner->GetPlayerAngle());
            break;
        case EffectType::jumpEffect:
            img = ImageManager::GetSingleton()->FindImage("jumpEffect");
            img->FrameRender(hdc, localPos.x, localPos.y, currFrame, 0, true,0.8f);
            break;
        case EffectType::flipEffect:
            img = ImageManager::GetSingleton()->FindImage("flipEffect");
            if(dir == 0)
                img->FrameRender(hdc, localPos.x, localPos.y, 1, currFrame, true);
            else
                img->FrameRender(hdc, localPos.x, localPos.y, 0, currFrame, true);
            break;
        case EffectType::dustEffect:
            img = ImageManager::GetSingleton()->FindImage("dustEffect");
            if (dir == 0) 
                img->FrameRender(hdc, localPos.x + moveSpeed, localPos.y - moveSpeed, currFrame, 0, true, size);
            else
                img->FrameRender(hdc, localPos.x - moveSpeed, localPos.y - moveSpeed, currFrame, 0, true, size);
            break;
        case EffectType::landEffect:
            img = ImageManager::GetSingleton()->FindImage("landEffect");
            img->FrameRender(hdc, localPos.x, localPos.y, currFrame, 0, true);
            break;
        case EffectType::reflectEffect:
            img = ImageManager::GetSingleton()->FindImage("reflectEffect");
            if (dir == 0)
                img->FrameRender(hdc, localPos.x - 100, localPos.y, currFrame, 0, true, size);
            else
                img->FrameRenderFlip(hdc, localPos.x, localPos.y, currFrame, 0, true, size);
            break;
        case EffectType::hitEffect:
            img = ImageManager::GetSingleton()->FindImage("hitEffect");
            img->rotateRender(hdc, localPos.x, localPos.y, 0, 0, owner->GetPlayerAngle(),2);
            break;
        }

    }
}

