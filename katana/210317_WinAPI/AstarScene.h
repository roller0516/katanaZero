#pragma once
#include "GameNode.h"

#define ASTAR_TILE_SIZE		60
#define ASTAR_TILE_COUNT	(ASTARSIZE_Y / ASTAR_TILE_SIZE)

enum class AstarTileType { Start, End, Wall, None };

class AstarTile : public GameNode
{
private:
	int idX, idY;
	POINT center;
	RECT rc;
	AstarTileType type;

	int costFromStart;	// g : 시작점부터 현재 노드까지의 비용
	int costToGoal;		// h : 현재 노드부터 목적지까지의 예상비용
	int totalCost;		// f : g + h

	AstarTile* parentTile;	// g가 갱신될 때마다 이전 노드를 갱신

	bool isInOpenlist;
	bool isClosed;

	COLORREF color;
	HBRUSH hBrush;
	HBRUSH hOldBrush;

	char szText[128];

	// heap 관련
	int heapIndex;

public:
	virtual HRESULT Init();
	HRESULT Init(int idX, int idY);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetColor(COLORREF color);
	void SetType(AstarTileType type) { this->type = type; }
	AstarTileType GetType() { return this->type; }
	int GetIdX() { return this->idX; }
	int GetIdY() { return this->idY; }

	void SetParentTile(AstarTile* parent) { this->parentTile = parent; }
	AstarTile* GetParentTile() { return this->parentTile; }

	void SetG(int g) { this->costFromStart = g; }
	int GetG() { return this->costFromStart; }

	void SetH(int h) { this->costToGoal = h; }
	int GetH() { return this->costToGoal; }

	void SetF(int f) { this->totalCost = f; }
	int GetF() { return this->totalCost; }

	POINT GetCenter() { return this->center; }

	void SetIsInOpenlist(bool b) { this->isInOpenlist = b; }
	bool GetIsInOpenlist() { return this->isInOpenlist; }
	void SetIsClosed(bool b) { this->isClosed = b; }
	bool GetIsClosed() { return this->isClosed; }

	void SetHeapIndex(int id) { this->heapIndex = id; }
	int GetHeapIndex() { return this->heapIndex; }

	virtual ~AstarTile() {};
};

class AstarScene : public GameNode
{
	// 이차원 배열 맵을 구성
	AstarTile map[ASTAR_TILE_COUNT][ASTAR_TILE_COUNT];

	AstarTile* startTile;	// 빨간색
	AstarTile* destTile;	// 파란색
	AstarTile* removeTile;
	AstarTile* currTile;	// (녹색) 후보타일을 선정할 때의 기준타일
	vector<AstarTile*> openList;
	vector<AstarTile*> closeList;
	vector<AstarTile*> heap;
	vector<AstarTile*> parentList;

	FPOINT pos;
	RECT rctest;
	RECT rcMain;

	FPOINT ptStartSelectedFrame;
	int ptSelected[3];
	int size;
	bool move;
	bool start;
	float angle;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void Move();
	void FindPath();
	void AddOpenList(AstarTile* currTile);
	AstarTile* GetMinTotalCostTile();
	// heap 사용
	AstarTile* GetMinTotalCostTileWithHeap();
	void UpdateLower(AstarTile* tile);
	void Swap(AstarTile* tile1, AstarTile* tile2);
	void InsertOpenlistWithHeap(AstarTile* tile);
	void UpdateUpper(AstarTile* tile);

	void DeleteTileInOpenlist(AstarTile* tile);
	int CalcEdgeCost(int x, int y);
	int CalcHeuristics(int x, int y);
	void MarkTileToType();

	virtual ~AstarScene() {};
};

