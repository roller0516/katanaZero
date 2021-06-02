#pragma once
#include "GameNode.h"
class Image;
class UI :
    public GameNode
{
private:
    Image* gui;
    Image* itmeIcon;
    Image* slowBar;
    Image* timerBar;
    Image* hudGo;
    Image* hud_go_arrow;

    FPOINT hudGoPos;
    FPOINT hud_go_arrowCurrPos;
    FPOINT guiPos;
    FPOINT hud_go_arrowPos;
    FPOINT itemIconPos;
    FPOINT slowBarPos;
    FPOINT timerBarPos;

    bool clear;
    float slowCoolTime;
    float timerCoolTime;
public:
    HRESULT Init();
    void Release();
    void Update();
    void Render(HDC hdc);
    void SetItemImage(string name);
    void SetClear(bool clear) { this->clear = clear; }
    Image* GetSlowImage() { return this->slowBar; }
};

