#pragma once
#include "GameNode.h"

// ���� Ÿ�� ����
// 640 * 288
#define SAMPLE_TILE_X	20
#define SAMPLE_TILE_Y	9
#define TILESIZE		32

// ���� �׸��� ���� ����
#define	TILE_X			20
#define	TILE_Y			20

typedef struct tagTile
{
	RECT rcTile;
	int frameX;
	int frameY;
} TILE_INFO;

class EnemyManager;
class Button;
class Image;
class TilemapTool : public GameNode
{
private:
	// ���� Ÿ�� �̹���
	FPOINT worldPos;
	FPOINT localPos;
	EnemyManager* enemyManager;
	Image* sampleTile;
	Image* BackGround;
	TILE_INFO sampleTileInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];
	RECT rcSample;

	// ����Ÿ���� ����
	static TILE_INFO tileInfo[TILE_X * TILE_Y];
	RECT rcMain;

	POINT ptStartSelectedFrame;
	POINT ptEndSelectedFrame;

	POINT ptSelected[2];
	HBRUSH hSelectedBrush;
	HBRUSH hOldSelectedBrush;

	// UI button
	Button* btnSave;
	Button* btnLoad;


public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	void CameraMove();

	FPOINT GetTileMapPos() { return this->worldPos; }

	static void Save(int stageNum);
	static void Load(int stageNum);

	virtual ~TilemapTool() {};
};

