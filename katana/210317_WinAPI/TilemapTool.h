#pragma once
#include "GameNode.h"

// 샘플 타일 정보
// 640 * 288
#define SAMPLE_TILE_X	20
#define SAMPLE_TILE_Y	9
#define TILESIZE		25

// 메인 그리기 공간 정보
#define	TILE_X			200
#define	TILE_Y			200
#define TILE_COUNT TILE_X*TILE_Y
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

static struct ENMY_INFO
{
	string Name = "";
	int x = 0;
	int y = 0;
	int index = 0;
};

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

	bool MonsterSpwan;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void EraseEnemy();
	void CameraMove();
	void SettingEnemy();

	void SetType(TILE_INFO* type, TileType tile) { type->type = tile; }
	TileType GetType(TILE_INFO* type) { return type->type; }

	void SetColor(TILE_INFO* type,bool wall);
	

	FPOINT GetTileMapPos() { return this->worldPos; }

	static void ChangeEnemy(int Index);
	static void NextPage(int Index);
	static void PrevPage(int Index);
	static void Save(int stageNum);
	static void Load(int stageNum);

	virtual ~TilemapTool() {};
};

