#include "installObject.h"
#include "Image.h"
#include "Camera.h"
HRESULT InstallObject::Init(int x, int y, InstallType Type)
{
   ImageManager::GetSingleton()->GetSingleton()->AddImage("Door", "Image/Katana/object/object_door_break_20x2.bmp", 3200, 256, 20, 2, true, RGB(255, 0, 255));
   ImageManager::GetSingleton()->GetSingleton()->AddImage("Flame", "Image/Katana/object/object_drum.bmp", 42, 54, 1, 1, true, RGB(255, 0, 255));
   this->Type = Type;
    switch (Type)
    {
    case InstallType::Door:
        img = ImageManager::GetSingleton()->FindImage("Door");
        close = true;
        break;
    case InstallType::Flame:
        img = ImageManager::GetSingleton()->FindImage("Flame");
        break;
    }

    worldPos.x = x;
    worldPos.y = y;

    size = 50;


    return S_OK;
}

void InstallObject::Release()
{

}

void InstallObject::Update()
{
    if (Type == InstallType::Door) 
    {
        if (oPen)
        {
            close = false;
            attackShape.left = (worldPos.x + 50) - (100) / 2;
            attackShape.top = worldPos.y - (100) / 2;
            attackShape.right = (worldPos.x + 50) + (100) / 2;
            attackShape.bottom = worldPos.y + (100) / 2;
            attackShape = { -100,-100,-100,-100 };
            shape = { -100,-100,-100,-100 };
            currframe += 15 * TimerManager::GetSingleton()->GetElapsedTime();
            if (currframe > 20)
            {
                currframe = 19;
                oPen = false;
            }
        }
        if (close)
        {
            shape.left = worldPos.x - 50 / 2;
            shape.top = worldPos.y - 50 / 2;
            shape.right = worldPos.x + 50 / 2;
            shape.bottom = worldPos.y + 50 / 2;

           
        }
    }
    
   

    localPos.x = worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
    localPos.y = worldPos.y - Camera::GetSingleton()->GetCameraPos().y;
}

void InstallObject::Render(HDC hdc)
{

    if (world) 
    {
        if (Type == InstallType::Door)
        {
            img->FrameRender(hdc, worldPos.x, worldPos.y, currframe, 0, true);
        }
        if (Type == InstallType::Flame)
            img->Render(hdc, worldPos.x, worldPos.y, true);
    }
    else 
    {
        if (Type == InstallType::Door)
        {
            img->FrameRender(hdc, localPos.x, localPos.y, currframe, 0, true);
        }
        if (Type == InstallType::Flame)
            img->Render(hdc, localPos.x, localPos.y, true);
    }
    Rectangle(hdc, shape.left - Camera::GetSingleton()->GetCameraPos().x, shape.top - Camera::GetSingleton()->GetCameraPos().y,
        shape.right - Camera::GetSingleton()->GetCameraPos().x, shape.bottom - Camera::GetSingleton()->GetCameraPos().y);
    Rectangle(hdc, attackShape.left - Camera::GetSingleton()->GetCameraPos().x, attackShape.top - Camera::GetSingleton()->GetCameraPos().y,
        attackShape.right - Camera::GetSingleton()->GetCameraPos().x, attackShape.bottom - Camera::GetSingleton()->GetCameraPos().y);
}
    

void InstallObject::AddObject()
{
    vObject.push_back(this->Clone());
}

InstallObject* InstallObject::Clone()
{
    return new InstallObject;
}
