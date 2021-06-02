#pragma once
#include "GameNode.h"

class Player;
class Image;
class Item :
    public GameNode
{
private:
    Image* img;
    Image* imgArrow;
    Player* owner;
    string name;
    FPOINT worldPos;
    FPOINT localPos;
    RECT hitShape;
    int alpha;
    int size;
    int itemIndex;
    float currFrame;
    float angle;
    float fireAngle;
    float moveSpeed;
    float alphaTime;
    int count;
    bool isFire;
    bool isAlive;
    bool getItem;
    bool arrowOn;
    bool isAlpha;
public:
    HRESULT Init(string name, int x, int y);
    void Release();
    void Update();
    void Render(HDC hdc);
    void Fire();
    void ChangeImage(bool change);
    Item* Clone();

    inline string GetName() {return this->name; }
    inline void SetRect() { this->hitShape = { -100,-100,-100,-100 }; }
    inline void SetAngle(float angle) { this->angle = angle; }
    inline void SetOwner(Player* owner) { this->owner = owner; }
    inline void SetAlive(bool alive) { this->isAlive = alive; }
    inline void SetisFire(bool fire) { this->isFire = fire; }
    inline void SetGetItem(bool getItem) { this->getItem = getItem; }
    inline bool GetItem() { return this->getItem; }
    inline bool Getfired() { return this->isFire; }
    inline void SetArrowOn(bool on) { this->arrowOn = on; }
    inline RECT GetRect() { return this->hitShape; }
    inline FPOINT GetWorldPos() { return this->worldPos; }

};

class ItemManager :
    public GameNode
{
private:
    string name;
    vector<Item*> vItem;
public:
    HRESULT Init(int x, int y,Player* owner);
    void Release();
    void Update();
    void Render(HDC hdc);

    void AddItem(string name, Item* item);
    vector<Item*> GetItemList() { return this->vItem; }
};

