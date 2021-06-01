#include "TilemapTool.h"
#include "Image.h"
#include "CommonFunction.h"
#include "Button.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "Enemy_Bold.h"
#include "Enemy_pomp.h"
#include "Enemy_Grunt.h"
#include "Enemy_Cop.h"
#include "AstarManager.h"
#include "installObject.h"
#include "ItemManager.h"

InstallObject* TilemapTool::installObj;
TILE_INFO TilemapTool::tileInfo[TILE_X * TILE_Y];
EnemyManager* TilemapTool::enemyManager;
Enemy* TilemapTool::exhibition;
ENMY_INFO TilemapTool::enemySize[100];
string TilemapTool::enenmyName;
int TilemapTool::changeIndex;
int TilemapTool::menuIndex;

HRESULT TilemapTool::Init()
{
    SetClientRect(g_hWnd, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y);

    enemyManager = new EnemyManager;
    enemyManager->RegisterClone("PompEnemy", new Enemy_pomp);
    enemyManager->RegisterClone("BoldEnemy", new Enemy_Bold);
    enemyManager->RegisterClone("GruntEnemy", new Enemy_Grunt);
    enemyManager->RegisterClone("CopEnemy", new Enemy_Cop);

    exhibition = enemyManager->CreateClone("PompEnemy");
    enenmyName = "PompEnemy";
    exhibition->Init(TILEMAPTOOLSIZE_X-200, TILEMAPTOOLSIZE_Y - 400);
    
    Camera::GetSingleton()->Init(this);
    worldPos.x = WINSIZE_X / 2;
    worldPos.y = WINSIZE_Y / 2;
    
    sampleTile = ImageManager::GetSingleton()->AddImage(
        "샘플타일", "Image/maptiles.bmp", 640, 288,
        SAMPLE_TILE_X, SAMPLE_TILE_Y);

    hSelectedBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

    rcSample = { 0,0,(int)Camera::GetSingleton()->GetWorld().x , (int)Camera::GetSingleton()->GetWorld().y };

    for (int i = 0; i < TILE_Y; i++)
    {
        for (int j = 0; j < TILE_X; j++)
        {
            tileInfo[i * TILE_X + j].rcTile.left = TILESIZE * j;
            tileInfo[i * TILE_X + j].rcTile.top = TILESIZE * i;
            tileInfo[i * TILE_X + j].rcTile.right = 
                tileInfo[i * TILE_X + j].rcTile.left + TILESIZE;
            tileInfo[i * TILE_X + j].rcTile.bottom =
                tileInfo[i * TILE_X + j].rcTile.top + TILESIZE;
            tileInfo[i * TILE_X + j].type = TileType::None;
            tileInfo[i * TILE_X + j].color = RGB(0, 0, 0);
            tileInfo[i * TILE_X + j].hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        }
    }

    // UI Button
    ImageManager::GetSingleton()->AddImage("아이템", "Image/button.bmp",
        122, 62, 1, 2);
    ImageManager::GetSingleton()->AddImage("몬스터", "Image/button.bmp",
        122, 62, 1, 2);
    ImageManager::GetSingleton()->AddImage("설치형오브젝트", "Image/button.bmp",
        122, 62, 1, 2);

    ImageManager::GetSingleton()->AddImage("저장버튼", "Image/button.bmp",
        122, 62, 1, 2);
    ImageManager::GetSingleton()->AddImage("불러오기버튼", "Image/button.bmp",
        122, 62, 1, 2);

    ImageManager::GetSingleton()->AddImage("Next", "Image/Katana/hud_go_arrow.bmp",
        62, 38, 1, 1,true,RGB(255,0,255));
    ImageManager::GetSingleton()->AddImage("Prev", "Image/Katana/hud_go_arrow.bmp",
        62, 38, 1, 1, true, RGB(255, 0, 255));

    changeIndex = 0;

    btnItem = new Button();
    btnItem->Init("아이템", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 550,
        TILEMAPTOOLSIZE_Y-400);
    btnItem->SetFunc(itemMenu, 1);

    btnMonster = new Button();
    btnMonster->Init("몬스터", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 550,
        TILEMAPTOOLSIZE_Y - 500);
    btnMonster->SetFunc(Monseter, 1);

    btnObject = new Button();
    btnObject->Init("설치형오브젝트", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 550,
        TILEMAPTOOLSIZE_Y - 600);
    btnObject->SetFunc(ObjectMenu, 1);

    btnSave = new Button();
    btnSave->Init("저장버튼", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 400,
        TILEMAPTOOLSIZE_Y - 200);
    btnSave->SetFunc(Save, 1);

    btnLoad = new Button();
    btnLoad->Init("불러오기버튼", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 550,
        TILEMAPTOOLSIZE_Y - 200);
    btnLoad->SetFunc(Load, 1);

    Next = new Button();
    Next->Init("Next", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 550,
        TILEMAPTOOLSIZE_Y - 300);
    Next->SetFunc(NextPage, 0);
    Prev = new Button();
    Prev->Init("Prev", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 400,
        TILEMAPTOOLSIZE_Y - 300);
    Prev->SetFunc(PrevPage, 1);

    //astarManager = new AstarManager;
    //astarManager->Init();

    

    rcMain = { 0,0,WINSIZE_X,WINSIZE_Y };
    ShowCursor(true);

    MonsterSpwan = false;

    installObj = new InstallObject;
    installObj->Init(0, 0, InstallType::Door);
    installObj->SetWrold(true);
    return S_OK;
}

void TilemapTool::Release()
{
    SAFE_RELEASE(btnSave);
    SAFE_RELEASE(btnLoad);
    SAFE_RELEASE(Next);
    SAFE_RELEASE(Prev);
    SAFE_RELEASE(enemyManager);
    SAFE_RELEASE(installObj);
    SAFE_RELEASE(btnObject);
    SAFE_RELEASE(btnItem);
    SAFE_RELEASE(btnMonster);
}

void TilemapTool::Update()
{
    installObj->Update();

    //if (menuIndex != 2) 
    //{
    //    astarManager = nullptr;
    //}


    


    exhibition->GetData()->astar = nullptr;

    CameraMove();
    Camera::GetSingleton()->Update();
    Camera::GetSingleton()->View();
    exhibition->Update();
    enemyManager->Update();
    if (btnSave)    btnSave->Update();
    if (btnLoad)    btnLoad->Update();
    if (Next) Next->Update();
    if (Prev) Prev->Update();
    if (btnMonster) btnMonster->Update();
    if (btnObject) btnObject->Update();
    if (btnItem) btnItem->Update();
    Prev->SetFunc(PrevPage, changeIndex);
    Next->SetFunc(NextPage, changeIndex);

    if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F9))
        SceneManager::GetSingleton()->ChangeScene("전투_1");
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_CONTROL))
    {
        if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))
        {
            Load(1);
        }
        else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))
        {
            Load(2);
        }
        else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F3))
        {
            Load(3);
        }
    }
    else
    {
        if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))
        {
            Save(1);
        }
        else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))
        {
            Save(2);
        }
        else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F3))
        {
            Save(3);
        }
    }

    if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F5)) 
    {
        MonsterSpwan = true;
        destTile = false;
    }
    if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F6)) 
    {
        MonsterSpwan = false;
        destTile = false;
    }   
    if (KeyManager::GetSingleton()->IsOnceKeyDown('2')) 
    {
        destTile = true;
        MonsterSpwan = false;
    }
       
    if (PtInRect(&rcMain, g_ptMouse))
    {
        // 마우스 왼쪽 버튼 클릭시 좌표 사용
        if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON)) 
        {
            if (menuIndex == 0)//아이템
            {

            }
            if (menuIndex == 1)//오브젝트
            {
                SettingObject();
            }
            if(menuIndex == 2 && MonsterSpwan) // 몬스터
                SettingEnemy();
 
        }
           
        if ((KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON) || KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON)) )
        {
            // g_ptMouse로 인덱스를 계산
            int x, y;
            x = GetWorldMousePos_p(worldPos).x / TILESIZE;
            y = GetWorldMousePos_p(worldPos).y / TILESIZE;

            if (menuIndex == 2 && MonsterSpwan == false) 
            {
                if (0 <= x && x < TILE_COUNT &&
                    0 <= y && y < TILE_COUNT)
                {
                    if (GetTileType(&tileInfo[y * TILE_X + x]) == TileType::None && destTile)
                    {
                        for (int i = 0; i < enemyManager->GetMonsterList().size(); i++)
                        {
                            enemyManager->GetMonsterList()[i]->GetData()->astar->SetDestTile(x, y);
                        }
                        SetColor(&tileInfo[y * TILE_X + x], RGB(0, 0, 255), false);
                        SetTileType(&tileInfo[y * TILE_X + x], TileType::End);
                    }
                    else if (GetTileType(&tileInfo[y * TILE_X + x]) != TileType::Start &&
                        GetTileType(&tileInfo[y * TILE_X + x]) != TileType::End)
                    {
                        SetColor(&tileInfo[y * TILE_X + x], RGB(255, 0, 0), false);
                        SetTileType(&tileInfo[y * TILE_X + x], TileType::Wall);
                    }
                }
            }
        }
        if(MonsterSpwan)
            EraseEnemy();

        if ((KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON) || KeyManager::GetSingleton()->IsStayKeyDown(VK_RBUTTON)) && MonsterSpwan == false)
        {
            // g_ptMouse로 인덱스를 계산
            int x, y;
            x = GetWorldMousePos_p(worldPos).x / TILESIZE;
            y = GetWorldMousePos_p(worldPos).y / TILESIZE;

            if (0 <= x && x < TILE_COUNT &&
                0 <= y && y < TILE_COUNT)
            {
                if (GetTileType(&tileInfo[y * TILE_X + x]) == TileType::Wall || GetTileType(&tileInfo[y * TILE_X + x]) == TileType::End)
                {
                    
                    SetColor(&tileInfo[y * TILE_X + x], RGB(0, 0, 0), true);
                    SetTileType(&tileInfo[y * TILE_X + x], TileType::None);
                }
            }
        }  
    }
    
   
    
    
}

void TilemapTool::Render(HDC hdc)
{
    PatBlt(hdc, 0, 0,
        TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y, WHITENESS);
    
    // 선택 영역 표시
    //hOldSelectedBrush = (HBRUSH)SelectObject(hdc, hSelectedBrush);
    //Rectangle(hdc, ptSelected[0].x, ptSelected[0].y, ptSelected[1].x, ptSelected[1].y);
    //SelectObject(hdc, hOldSelectedBrush);
    // 메인영역 전체

    Camera::GetSingleton()->Render(hdc,"stage1_bg_render");
    

    // UI Button
    if (btnSave)    btnSave->Render(hdc);
    if (btnLoad)    btnLoad->Render(hdc);
    if (btnItem) btnItem->Render(hdc);
    if (btnMonster) btnMonster->Render(hdc);
    if (btnObject) btnObject->Render(hdc);
    if (Next) Next->Render(hdc);
    if (Prev)  Prev->Render(hdc, true);
    for (int i = 0; i < TILE_X * TILE_Y; i++)
    {
        if (tileInfo[i].rcTile.left - Camera::GetSingleton()->GetCameraPos().x > WINSIZE_X)
            continue;
        if (tileInfo[i].rcTile.top - Camera::GetSingleton()->GetCameraPos().y > WINSIZE_Y)
            continue;
        tileInfo[i].hOldBrush = (HBRUSH)SelectObject(hdc, tileInfo[i].hBrush);
        Rectangle(hdc,
            tileInfo[i].rcTile.left - Camera::GetSingleton()->GetCameraPos().x,
            tileInfo[i].rcTile.top - Camera::GetSingleton()->GetCameraPos().y,
            tileInfo[i].rcTile.right - Camera::GetSingleton()->GetCameraPos().x,
            tileInfo[i].rcTile.bottom - Camera::GetSingleton()->GetCameraPos().y);
        SelectObject(hdc, tileInfo[i].hOldBrush);
    }
    if(menuIndex == 0)
        installObj->Render(hdc);

    //if (astarManager)
    //    astarManager->Render(hdc);

    sprintf_s(szText, "playerX : %d , playerY : %d", g_ptMouse.x, g_ptMouse.y);
    TextOut(hdc, WINSIZE_X - 800, 20, szText, strlen(szText));
    sprintf_s(szText, "X : %f, Y : %f", GetWorldMousePos(worldPos).x, GetWorldMousePos(worldPos).y);
    TextOut(hdc, 200, 20, szText, strlen(szText));
    if (menuIndex == 2) 
    {
        enemyManager->Render(hdc);
        exhibition->Render(hdc, true);
    }  
}

void TilemapTool::EraseEnemy()
{
    if (enemyManager->GetMonsterList().size() == 0)
        return;
    RECT rc;
    POINT worldMouse;
    worldMouse.x = Camera::GetSingleton()->GetWorldMousePos().x;
    worldMouse.y = Camera::GetSingleton()->GetWorldMousePos().y;
    for (int i = 0; i < enemyManager->GetMonsterList().size(); i++) 
    {
        rc = enemyManager->GetMonsterList()[i]->GetData()->shape;
        if (PtInRect(&rc, worldMouse))
        {
            if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON)) 
            {
                string str = "";
                enemyManager->GetMonsterList()[i]->GetData()->astar = nullptr;
                //enemyManager->GetMonsterList()[i]->GetData()->astar->SetTarget(nullptr);
                //enemyManager->GetMonsterList()[i]->GetData()->astar->SetOnwer(nullptr);
                enemyManager->DeletEnemy(i);
                strcpy_s(enemySize[i].Name, str.c_str());
                enemySize[i].x = 0;
                enemySize[i].y = 0;
                enemySize[i].index = 0;
                break;
            }
        }
    }
}

void TilemapTool::CameraMove()
{
    float time = TimerManager::GetSingleton()->GetElapsedTime();
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RIGHT)) 
        worldPos.x += time * 1000;
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LEFT))
        worldPos.x -= time * 1000;
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_UP))
        worldPos.y -= time * 1000;
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_DOWN))
        worldPos.y += time * 1000;
}

void TilemapTool::SettingEnemy()
{
    monsterCount = enemyManager->GetMonsterList().size();
    enemyManager->AddEnemy(enenmyName,1);
    enemyManager->Init(nullptr, Camera::GetSingleton()->GetWorldMousePos().x, Camera::GetSingleton()->GetWorldMousePos().y, monsterCount);
    strcpy_s(enemySize[enemyManager->GetMonsterList().size() - 1].Name, enenmyName.c_str());
    enemySize[enemyManager->GetMonsterList().size() - 1].x = Camera::GetSingleton()->GetWorldMousePos().x;
    enemySize[enemyManager->GetMonsterList().size() - 1].y = Camera::GetSingleton()->GetWorldMousePos().y;
    enemySize[enemyManager->GetMonsterList().size() - 1].index = monsterCount;
    monsterCount++;
}

void TilemapTool::SettingObject()
{
    vObject.push_back(installObj->Clone());
    for (int i = 0; i < vObject.size(); i++) 
    {
        vObject[i]->Init(Camera::GetSingleton()->GetWorldMousePos().x, 
            Camera::GetSingleton()->GetWorldMousePos().y, (InstallType)(changeIndex - 1));
    }
}


void TilemapTool::SettingItem()
{

}


void TilemapTool::ChangeEnemy(int Index)
{
    switch (Index)
    {
    case 0:
        exhibition = enemyManager->CreateClone("PompEnemy");
        exhibition->Init(TILEMAPTOOLSIZE_X - 200, TILEMAPTOOLSIZE_Y - 400);
        exhibition->GetData()->moveSpeed = 0;
        exhibition->GetData()->isSamPle = true;
        enenmyName = "PompEnemy";
        break;
    case 1:
        exhibition = enemyManager->CreateClone("BoldEnemy");
        exhibition->Init(TILEMAPTOOLSIZE_X - 200, TILEMAPTOOLSIZE_Y - 400);
        exhibition->GetData()->moveSpeed = 0;
        exhibition->GetData()->isSamPle = true;
        enenmyName = "BoldEnemy";
        break;
    case 2:
        exhibition = enemyManager->CreateClone("GruntEnemy");
        exhibition->Init(TILEMAPTOOLSIZE_X - 200, TILEMAPTOOLSIZE_Y - 400);
        exhibition->GetData()->moveSpeed = 0;
        exhibition->GetData()->isSamPle = true;
        enenmyName = "GruntEnemy";
        break;
    case 3:
        exhibition = enemyManager->CreateClone("CopEnemy");
        exhibition->Init(TILEMAPTOOLSIZE_X - 200, TILEMAPTOOLSIZE_Y - 400);
        exhibition->GetData()->moveSpeed = 0;
        exhibition->GetData()->isSamPle = true;
        enenmyName = "CopEnemy";
        break;
    }
}
void TilemapTool::ChangeItem(int Index)
{

}
void TilemapTool::ChangeObject(int Index)
{
    switch (Index) 
    {
    case 1:
        installObj->Init(TILEMAPTOOLSIZE_X - 200, TILEMAPTOOLSIZE_Y - 400, InstallType::Door);
        break;
    case2:
        installObj->Init(TILEMAPTOOLSIZE_X - 200, TILEMAPTOOLSIZE_Y - 400, InstallType::Flame);
        break;
    }
}
void TilemapTool::SetColor(TILE_INFO* tile, COLORREF color, bool nullcolor)
{
    tile->color = color;
    DeleteObject(tile->hBrush);
    if (nullcolor)
        tile->hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    else
        tile->hBrush = CreateSolidBrush(color);
}
void TilemapTool::NextPage(int Index)
{
    if (menuIndex == 0) // 아이템
    {
        if (changeIndex == 4)
            changeIndex = 4;
        else
            changeIndex++;
    }
    if (menuIndex == 1) 
    {
        if (changeIndex == 2)
            changeIndex = 2;
        else
            changeIndex++;
        ChangeObject(changeIndex);
    } // 오브젝트
            
    if (menuIndex == 2) // 몬스터
    {
        if (changeIndex == 3)
            changeIndex = 3;
        else
            changeIndex++;

        ChangeEnemy(changeIndex);
    }
}

void TilemapTool::PrevPage(int Index)
{
    if (menuIndex == 0)
    {
        if (changeIndex == 0)
            changeIndex = 0;
        else
            changeIndex--;

        ChangeEnemy(changeIndex);
    }
    if (menuIndex == 1)
    {
        if (changeIndex == 0)
            changeIndex = 0;
        else
            changeIndex--;

        ChangeEnemy(changeIndex);
    }
    if (menuIndex == 2) 
    {
        if (changeIndex == 0)
            changeIndex = 0;
        else
            changeIndex--;

        ChangeEnemy(changeIndex);
    }
}

/*
    실습1. F1, F2, F3  각 키를 눌렀을 때 
    Save/saveMapData1.map, Save/saveMapData2.map, Save/saveMapData3.map
    각 각 파일에 저장될 수 있도록 코드 구현

    실습2. 로드는 Ctrl + F1, ... 
*/

void TilemapTool::Save(int stageNum)
{
    string fileName = "Save/saveMapData";  // 1.map";
    fileName += to_string(stageNum) + ".map";
    string fileName1 = "Save/saveMapDataTile";  // 1.map";
    fileName1 += to_string(stageNum) + ".map";
    DWORD writtenBytes[2];
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0,
        0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    HANDLE hFile2 = CreateFile(fileName1.c_str(), GENERIC_WRITE, 0,
        0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    /*void**/

    WriteFile(hFile, enemySize, sizeof(ENMY_INFO) * 100,
            &writtenBytes[0], NULL);

    WriteFile(hFile2, tileInfo,  sizeof(TILE_INFO)*TILE_X*TILE_Y,
        &writtenBytes[1], NULL);

    CloseHandle(hFile2);
    CloseHandle(hFile);
}

void TilemapTool::Load(int stageNum)
{
    string fileName = "Save/saveMapData";  // 1.map";
    fileName += to_string(stageNum) + ".map";
    string fileName1 = "Save/saveMapDataTile";  // 1.map";
    fileName1 += to_string(stageNum) + ".map";
    DWORD readBytes[2];
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0,
        0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    HANDLE hFile2 = CreateFile(fileName1.c_str(), GENERIC_READ, 0,
        0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (ReadFile(hFile, enemySize, sizeof(ENMY_INFO) * 100,
        &readBytes[0], NULL))
    {
        for (int i = 0; i < 100; i++)
        {
            string str(enemySize[i].Name);
            if (str == "")
                break;
            enemyManager->AddEnemy(str, 1);
            enemyManager->Init(nullptr, enemySize[i].x, enemySize[i].y, enemySize[i].index);
        }
        

    }
    else
    {
        MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);
    }

    if (ReadFile(hFile2, tileInfo, sizeof(TILE_INFO)*TILE_X*TILE_Y,
        &readBytes[1], NULL)) 
    {
        for (int i = 0; i < TILE_Y; i++)
        {
            for (int j = 0; j < TILE_X; j++)
            {
                if(tileInfo[i * TILE_X + j].color)
                    tileInfo[i * TILE_X + j].hBrush = CreateSolidBrush(tileInfo[i * TILE_X + j].color);
            }
        }
    }
    else
    {
        MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);
    }

    if (enemyManager)
    {
        for (int i = 0; i < enemyManager->GetMonsterList().size(); i++)
        {
            for (int j = 0; j < TILE_Y; j++)
            {
                for (int k = 0; k < TILE_X; k++)
                {
                    if (tileInfo[j * TILE_X + k].type == TileType::Wall)
                        enemyManager->GetMonsterList()[i]->GetData()->astar->SetWall(j, k);
                }
            }

            if (enemyManager->GetMonsterList().size() > 0)
            {
                enemyManager->GetMonsterList()[i]->GetData()->astar->SetOnwer(enemyManager->GetMonsterList()[i]);
            }
        }
    }
    

    CloseHandle(hFile2);
    CloseHandle(hFile);
}

void TilemapTool::itemMenu(int Index)
{
    menuIndex = 0;
    
}

void TilemapTool::ObjectMenu(int Index)
{
    menuIndex = 1;
}

void TilemapTool::Monseter(int Index)
{
    menuIndex = 2;
}


