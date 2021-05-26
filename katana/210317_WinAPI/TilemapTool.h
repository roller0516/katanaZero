#pragma once
#include "GameNode.h"

// 샘플 타일 정보
// 640 * 288
#define SAMPLE_TILE_X	20
#define SAMPLE_TILE_Y	9


enum class TileType { Start, End, Wall, None };

typedef struct tagTile
{
	RECT rcTile;
	int frameX;
	int frameY;
	TileType type;
	COLORREF color;
	HBRUSH hBrush;
	HBRUSH hOldBrush;
} TILE_INFO;

typedef struct ENMY_INFO
{
	string Name;
	int x;
	int y;
	int index;
	ENMY_INFO() 
	{
		Name = "";
		x = 0;
		y = 0;
		index = 0;
	}
};

class AstarManager;
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
	static string enenmyName;
	static ENMY_INFO enemySize[100];

	FPOINT worldPos;
	FPOINT localPos;

	Image* sampleTile;
	Image* BackGround;
	RECT rcSample;

	// 메인타일의 정보
	static TILE_INFO tileInfo[TILE_X * TILE_Y];
	AstarManager* astarManager;
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

	bool destTile;
	bool MonsterSpwan;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void EraseEnemy();
	void CameraMove();
	void SettingEnemy();

	FPOINT GetTileMapPos() { return this->worldPos; }

	static void ChangeEnemy(int Index);
	static void NextPage(int Index);
	static void PrevPage(int Index);
	static void Save(int stageNum);
	static void Load(int stageNum);
	void SetColor(TILE_INFO* tile ,COLORREF color, bool nullcolor);
	void SetTileType(TILE_INFO* tile, TileType type) { tile->type = type; }
	TileType GetTileType(TILE_INFO* tile) { return tile->type; }
	virtual ~TilemapTool() {};
};

