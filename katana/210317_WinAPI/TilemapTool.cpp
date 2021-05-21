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

TILE_INFO TilemapTool::tileInfo[TILE_X * TILE_Y];
EnemyManager* TilemapTool::enemyManager;
Enemy* TilemapTool::exhibition;
string TilemapTool::MonsterName;
int TilemapTool::changeIndex;
ENMY_INFO TilemapTool::test2[100];
HRESULT TilemapTool::Init()
{
    SetClientRect(g_hWnd, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y);

    enemyManager = new EnemyManager;
    enemyManager->RegisterClone("PompEnemy", new Enemy_pomp);
    enemyManager->RegisterClone("BoldEnemy", new Enemy_Bold);
    enemyManager->RegisterClone("GruntEnemy", new Enemy_Grunt);
    enemyManager->RegisterClone("CopEnemy", new Enemy_Cop);

    exhibition = enemyManager->CreateClone("PompEnemy");
    MonsterName = "PompEnemy";
    exhibition->Init(TILEMAPTOOLSIZE_X-200, TILEMAPTOOLSIZE_Y - 400);
    
    
    Camera::GetSingleton()->Init(this);
    worldPos.x = WINSIZE_X / 2;
    worldPos.y = WINSIZE_Y / 2;
    
    sampleTile = ImageManager::GetSingleton()->AddImage(
        "샘플타일", "Image/maptiles.bmp", 640, 288,
        SAMPLE_TILE_X, SAMPLE_TILE_Y);

    hSelectedBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    rcSample = { 0,0,(int)Camera::GetSingleton()->GetWorld().x , (int)Camera::GetSingleton()->GetWorld().y };
    // 메인 공간 렉트 설정
   /* for (int i = 0; i < TILE_Y; i++)
    {
        for (int j = 0; j < TILE_X; j++)
        {
            tileInfo[i * TILE_X + j].frameX = 3;
            tileInfo[i * TILE_X + j].frameY = 0;

            tileInfo[i * TILE_X + j].rcTile.left = TILESIZE * j;
            tileInfo[i * TILE_X + j].rcTile.top = TILESIZE * i;
            tileInfo[i * TILE_X + j].rcTile.right = 
                tileInfo[i * TILE_X + j].rcTile.left + TILESIZE;
            tileInfo[i * TILE_X + j].rcTile.bottom = 
                tileInfo[i * TILE_X + j].rcTile.top + TILESIZE;
        }
    }*/

    // 샘플 공간 렉트 설정
    //for (int i = 0; i < SAMPLE_TILE_Y; i++)
    //{
    //    for (int j = 0; j < SAMPLE_TILE_X; j++)
    //    {
    //        SetRect(&sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile,
    //            TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + (TILESIZE * j),
    //            (TILESIZE * i),
    //            TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + (TILESIZE * j) + TILESIZE,
    //            (TILESIZE * i) + TILESIZE);

    //        sampleTileInfo[i * SAMPLE_TILE_X + j].frameX = j;
    //        sampleTileInfo[i * SAMPLE_TILE_X + j].frameY = i;

    //        //sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile.left =
    //        //    TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + (TILESIZE * j);
    //        //sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile.top = (TILESIZE * i);
    //        //sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile.right =
    //        //    sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile.left + TILESIZE;
    //        //sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile.bottom =
    //        //    sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile.top + TILESIZE;
    //    }
    //}
   
    // UI Button
    ImageManager::GetSingleton()->AddImage("저장버튼", "Image/button.bmp",
        122, 62, 1, 2);
    ImageManager::GetSingleton()->AddImage("불러오기버튼", "Image/button.bmp",
        122, 62, 1, 2);

    ImageManager::GetSingleton()->AddImage("Next", "Image/Katana/hud_go_arrow.bmp",
        62, 38, 1, 1,true,RGB(255,0,255));
    ImageManager::GetSingleton()->AddImage("Prev", "Image/Katana/hud_go_arrow.bmp",
        62, 38, 1, 1, true, RGB(255, 0, 255));

    changeIndex = 0;

    btnSave = new Button();
    btnSave->Init("저장버튼", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 400,
        TILEMAPTOOLSIZE_Y - 200);
    btnSave->SetFunc(Save, 1);

    btnLoad = new Button();
    btnLoad->Init("불러오기버튼", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 550,
        TILEMAPTOOLSIZE_Y - 200);
    btnLoad->SetFunc(Load, 1);

    Next = new Button();
    Next->Init("Next", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 400,
        TILEMAPTOOLSIZE_Y - 300);
    Next->SetFunc(NextPage, 0);
    Prev = new Button();
    Prev->Init("Prev", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 550,
        TILEMAPTOOLSIZE_Y - 300);
    Prev->SetFunc(PrevPage, 1);
    return S_OK;
}

void TilemapTool::Release()
{
    SAFE_RELEASE(btnSave);
    SAFE_RELEASE(btnLoad);
    SAFE_RELEASE(Next);
    SAFE_RELEASE(Prev);
    SAFE_RELEASE(enemyManager);
}

void TilemapTool::Update()
{
    CameraMove();
    Camera::GetSingleton()->Update();
    Camera::GetSingleton()->View();
    exhibition->Update();
    enemyManager->Update();
    
    if (btnSave)    btnSave->Update();
    if (btnLoad)    btnLoad->Update();
    if (Next) Next->Update();
    if (Prev) Prev->Update();

    Prev->SetFunc(NextPage, changeIndex);
    Next->SetFunc(PrevPage, changeIndex);

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

    // 메인 영역 계산
    rcMain.left = 0;
    rcMain.top = 0;
    rcMain.right = WINSIZE_X;
    rcMain.bottom = WINSIZE_Y;

    //// 샘플 영역 계산
    //rcSample.left = TILEMAPTOOLSIZE_X - sampleTile->GetWidth();
    //rcSample.top = 0;
    //rcSample.right = TILEMAPTOOLSIZE_X;
    //rcSample.bottom = sampleTile->GetHeight();

    if (PtInRect(&rcMain, g_ptMouse))
    {
        // 마우스 왼쪽 버튼 클릭시 좌표 사용
        if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
        {
            SettingEnemy();

            //for (int i = 0; i < TILE_X * TILE_Y; i++)
            //{
            //    if (PtInRect(&(tileInfo[i].rcTile), g_ptMouse))
            //    {
            //        //selectedFrameX = i % TILE_X;
            //        //selectedFrameY = i / TILE_X;

            //        tileInfo[i].frameX = ptStartSelectedFrame.x;
            //        tileInfo[i].frameY = ptStartSelectedFrame.y;

            //        for (int j = 0; j <= ptEndSelectedFrame.y - ptStartSelectedFrame.y; j++)
            //        {
            //            for (int k = 0; k <= ptEndSelectedFrame.x - ptStartSelectedFrame.x; k++)
            //            {
            //                if ((i % TILE_X) + k >= TILE_X) continue;
            //                if ((i / TILE_X) + j >= TILE_Y) continue;

            //                tileInfo[i + j * TILE_X + k].frameX = ptStartSelectedFrame.x + k;
            //                tileInfo[i + j * TILE_X + k].frameY = ptStartSelectedFrame.y + j;
            //            }
            //        }

            //        break;
            //    }
            //}
        }
    }
    else if (PtInRect(&rcSample, g_ptMouse))
    {
        // 마우스 왼쪽 버튼 클릭시 좌표 사용
        if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
        {

            // 2) 마우스 좌표로 인덱스 계산
            int posX = g_ptMouse.x - rcSample.left;
            int posY = g_ptMouse.y - rcSample.top;
            //ptStartSelectedFrame.x = posX / TILESIZE;
            //ptStartSelectedFrame.y = posY / TILESIZE;

            ptSelected[0] = g_ptMouse;

            //// 1) 모든 타일을 반복하면서 렉트충돌 확인
            //for (int i = 0; i < SAMPLE_TILE_X * SAMPLE_TILE_Y; i++)
            //{
            //    if (PtInRect(&(sampleTileInfo[i].rcTile), g_ptMouse))
            //    {
            //        ptStartSelectedFrame.x = i % SAMPLE_TILE_X;
            //        ptStartSelectedFrame.y = i / SAMPLE_TILE_X;

            //        break;
            //    }
            //}
        }
        else if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
        {
            int posX = g_ptMouse.x - rcSample.left;
            int posY = g_ptMouse.y - rcSample.top;
            ptEndSelectedFrame.x = posX / TILESIZE;
            ptEndSelectedFrame.y = posY / TILESIZE;

            // 선택영역 초기화
            ptSelected[0].x = -1;
            ptSelected[0].y = -1;
            ptSelected[1].x = -1;
            ptSelected[1].y = -1;
        }
        else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
        {
            ptSelected[1] = g_ptMouse;
        }
    }

}

void TilemapTool::Render(HDC hdc)
{
    PatBlt(hdc, 0, 0,
        TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y, WHITENESS);

    // 샘플타일 전체
    sampleTile->Render(hdc, TILEMAPTOOLSIZE_X - sampleTile->GetWidth(), 0);

    // 선택 영역 표시
    hOldSelectedBrush = (HBRUSH)SelectObject(hdc, hSelectedBrush);
    Rectangle(hdc, ptSelected[0].x, ptSelected[0].y, ptSelected[1].x, ptSelected[1].y);
    SelectObject(hdc, hOldSelectedBrush);

    // UI Button
    if (btnSave)    btnSave->Render(hdc);
    if (btnLoad)    btnLoad->Render(hdc);
    if (Next) Next->Render(hdc);
    if (Prev)  Prev->Render(hdc);
    // 메인영역 전체
    for (int i = 0; i < TILE_X * TILE_Y; i++)
    {
        sampleTile->FrameRender(hdc,
            tileInfo[i].rcTile.left,
            tileInfo[i].rcTile.top,
            tileInfo[i].frameX,
            tileInfo[i].frameY);
    }
    Camera::GetSingleton()->Render(hdc);
    
    // 선택된 타일
  /*  if (ptStartSelectedFrame.x == ptEndSelectedFrame.x &&
        ptStartSelectedFrame.y == ptEndSelectedFrame.y)
    {
        sampleTile->FrameRender(hdc, 
            TILEMAPTOOLSIZE_X - sampleTile->GetWidth(),
            sampleTile->GetHeight() + 50, 
            ptStartSelectedFrame.x, ptStartSelectedFrame.y, false, 3);
    }
    else
    {
        for (int i = 0; i <= ptEndSelectedFrame.y - ptStartSelectedFrame.y; i++)
        {
            for (int j = 0; j <= ptEndSelectedFrame.x - ptStartSelectedFrame.x; j++)
            {
                sampleTile->FrameRender(hdc,
                    TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + (j * TILESIZE),
                    sampleTile->GetHeight() + 50 + (i * TILESIZE),
                    ptStartSelectedFrame.x + j, ptStartSelectedFrame.y + i, false, 1);

            }
        }
    }*/

    sprintf_s(szText, "playerX : %d , playerY : %d", g_ptMouse.x, g_ptMouse.y);
    TextOut(hdc, WINSIZE_X - 800, 20, szText, strlen(szText));
    sprintf_s(szText, "X : %f, Y : %f", GetWorldMousePos(worldPos).x, GetWorldMousePos(worldPos).y);
    TextOut(hdc, 200, 20, szText, strlen(szText));

    enemyManager->Render(hdc);
    exhibition->Render(hdc,true);
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
    float x, y;
    x = Camera::GetSingleton()->GetWorldMousePos(this->worldPos).x;
    y = Camera::GetSingleton()->GetWorldMousePos(this->worldPos).y;
    enemyManager->AddEnemy(MonsterName,1);
    enemyManager->Init(nullptr,x, y, monsterCount);
    test2[enemyManager->GetMonsterList().size() - 1].Name = MonsterName;
    test2[enemyManager->GetMonsterList().size() - 1].x = x;
    test2[enemyManager->GetMonsterList().size() - 1].y = y;
    test2[enemyManager->GetMonsterList().size() - 1].index = monsterCount;
    monsterCount++;
}

void TilemapTool::ChangeEnemy(int Index)
{
    switch (Index)
    {
    case 0:
        exhibition = enemyManager->CreateClone("PompEnemy");
        exhibition->Init(TILEMAPTOOLSIZE_X - 200, TILEMAPTOOLSIZE_Y - 400);
        exhibition->SetMoveSpeed(0);
        exhibition->SetSample(true);
        MonsterName = "PompEnemy";
        break;
    case 1:
        exhibition = enemyManager->CreateClone("BoldEnemy");
        exhibition->Init(TILEMAPTOOLSIZE_X - 200, TILEMAPTOOLSIZE_Y - 400);
        exhibition->SetMoveSpeed(0);
        exhibition->SetSample(true);
        MonsterName = "BoldEnemy";
        break;
    case 2:
        exhibition = enemyManager->CreateClone("GruntEnemy");
        exhibition->Init(TILEMAPTOOLSIZE_X - 200, TILEMAPTOOLSIZE_Y - 400);
        exhibition->SetMoveSpeed(0);
        exhibition->SetSample(true);
        MonsterName = "GruntEnemy";
        break;
    case 3:
        exhibition = enemyManager->CreateClone("CopEnemy");
        exhibition->Init(TILEMAPTOOLSIZE_X - 200, TILEMAPTOOLSIZE_Y - 400);
        exhibition->SetMoveSpeed(0);
        exhibition->SetSample(true);
        MonsterName = "CopEnemy";
        break;
    }
}

void TilemapTool::NextPage(int Index)
{
    if (changeIndex == 3)
        changeIndex = 3;
    else
        changeIndex++;

    ChangeEnemy(changeIndex);
}

void TilemapTool::PrevPage(int Index)
{
    if (changeIndex == 0)
        changeIndex = 0;
    else
        changeIndex--;
    ChangeEnemy(changeIndex);
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
    DWORD writtenBytes;
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0,
        0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    /*void**/
    WriteFile(hFile, test2, sizeof(ENMY_INFO) * 100,
            &writtenBytes, NULL);
    CloseHandle(hFile);
}

void TilemapTool::Load(int stageNum)
{
    string fileName = "Save/saveMapData";  // 1.map";
    fileName += to_string(stageNum) + ".map";

    DWORD readBytes;
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0,
        0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    for (int i = 0; i < 100; i++) 
    {
        if (ReadFile(hFile, &test2[i], sizeof(ENMY_INFO) * 100,
            &readBytes, NULL))
        {
            if (test2[i].Name == "")
                break;
            enemyManager->AddEnemy(test2[i].Name,1);
            enemyManager->Init(nullptr,test2[i].x, test2[i].y, test2[i].index);
        }
        else
        {
            MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);
        }
    }
    CloseHandle(hFile);
}
