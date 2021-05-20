#pragma once
#include "GameNode.h"

// 샘플 타일 정보
// 640 * 288
#define SAMPLE_TILE_X	20
#define SAMPLE_TILE_Y	9
#define TILESIZE		32

// 메인 그리기 공간 정보
#define	TILE_X			20
#define	TILE_Y			20

typedef struct tagTile
{
	RECT rcTile;
	int frameX;
	int frameY;
} TILE_INFO;

typedef struct tagEnemy
{
	string Name;
	int x;
	int y;
	int index;
} ENMY_INFO;

class Enemy;
class EnemyManager;
class Button;
class Image;
class TilemapTool : public GameNode
{
private:
	// 샘플 타일 이미지
	char szText[128];

	static EnemyManager* enemyManager;
	static Enemy* exhibition;
	static string MonsterName;
	static ENMY_INFO test2[100];
	FPOINT worldPos;
	FPOINT localPos;

	Image* sampleTile;
	Image* BackGround;
	TILE_INFO sampleTileInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];
	RECT rcSample;

	// 메인타일의 정보
	static TILE_INFO tileInfo[TILE_X * TILE_Y];
	RECT rcMain;

	POINT ptStartSelectedFrame;
	POINT ptEndSelectedFrame;

	POINT ptSelected[2];
	HBRUSH hSelectedBrush;
	HBRUSH hOldSelectedBrush;

	int monsterCount;
	static int changeIndex;
	// UI button
	Button* btnSave;
	Button* btnLoad;

	Button* Next;
	Button* Prev;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void CameraMove();
	void SettingEnemy();


	FPOINT GetTileMapPos() { return this->worldPos; }

	static void ChangeEnemy(int Index);
	static void NextPage(int Index);
	static void PrevPage(int Index);
	static void Save(int stageNum);
	static void Load(int stageNum);

	virtual ~TilemapTool() {};
};

