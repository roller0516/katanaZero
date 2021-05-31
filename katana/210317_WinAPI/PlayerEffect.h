#pragma once
#include "GameNode.h"
enum class EffectType{attackEffect, jumpEffect,flipEffect,landEffect, dustEffect, reflectEffect,hitEffect};
class Image;
class Player;
class PlayerEffect :
    public GameNode
{
private:
    Image* img;
    Player* owner;
    FPOINT worldPos;
    FPOINT localPos;
    float currFrame;
    float maxFrame;
    bool isAlive;
    bool isFollow;
    float size;
    float moveSpeed;
    int count;
    string name;
    EffectType effectType;
    //map<EffectType, PlayerEffect*> vPlayerEffect;
public:
    HRESULT Init(EffectType tpye,Player* player);
    void Release();
    void Update();
    void Render(HDC hdc);

    void SetWorldPos(int x, int y) { this->worldPos.x = x; this->worldPos.y = y; }
    void SetCurrFrame() { this->currFrame = 0; }
    void SetAlive(bool alive) { this->isAlive = alive; }
    void SetSize(float size) { this->size = size; }
    bool GetAlive() { return this->isAlive; }
    //map<EffectType, PlayerEffect*> GetPlayerEffect() { return this->vPlayerEffect; }
};

