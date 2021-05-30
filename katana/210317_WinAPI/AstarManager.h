#pragma once
#include "GameNode.h"

enum class AstarTileType { Start, End, Wall, None ,Parent};

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
	HRESULT Init();
	HRESULT Init(int idX, int idY);
	void Release();
	void Update();
	void Render(HDC hdc);
	void Clear();

	void SetColor(COLORREF color,bool nullcolor);
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

class Player;
class Enemy;
class AstarManager : public GameNode
{
	AstarTile Map[TILE_X][TILE_Y];

	AstarTile* startTile;	// 빨간색
	AstarTile* destTile;	// 파란색
	AstarTile* backTile;
	AstarTile* currTile;	// (녹색) 후보타일을 선정할 때의 기준타일

	vector<AstarTile*> openList;
	vector<AstarTile*> closeList;
	vector<AstarTile*> heap;
	map<int, vector<AstarTile*>> parentList;

	Player* target;
	Enemy* owner;
	FPOINT pos;
	RECT rctest;
	RECT rcMain;

	FPOINT onwerTileIndex;
	FPOINT targetTileIndex;
	int ptSelected[3];
	int size;
	int count;
	bool move;
	bool isFind;
	bool start;
	float angle;
	int index;
public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void Move();
	void FindPath();
	void AddOpenList(AstarTile* currTile);
	AstarTile* GetMinTotalCostTile();
	AstarTile* GetMap(int x, int y) { return &Map[y][x]; }
	// heap 사용
	AstarTile* GetMinTotalCostTileWithHeap();
	void UpdateLower(AstarTile* tile);
	void Swap(AstarTile* tile1, AstarTile* tile2);
	void InsertOpenlistWithHeap(AstarTile* tile);
	void UpdateUpper(AstarTile* tile);
	void SetRect(RECT rc) { this->rcMain = rc; }
	void SetWall(int x, int y) { Map[x][y].SetType(AstarTileType::Wall); }
	void AddAtsar(int index);
	void DeleteTileInOpenlist(AstarTile* tile);
	void MarkTileToType(int index);
	vector<AstarTile*>* GetParentList(int index);

	int CalcEdgeCost(int x, int y);
	int CalcHeuristics(int x, int y);
	void ParentPopBack(int index);
	void Clear();
	
	void SetDestTile(int x, int y);

	inline AstarTile* GetDestTile() { return this->destTile; }
	inline void SetBackTile(AstarTile* astar) { this->backTile = astar; }
	inline FPOINT GetTileIndex() { return this->onwerTileIndex; }
	inline void SetGetTileIndex(int x, int y) { onwerTileIndex.x = x; onwerTileIndex.y; }
	inline AstarTile* GetBackTile() {return this->backTile;}
	inline RECT GetMainRect() { return this->rcMain; }
	inline void SetStartTile(int x, int y) { startTile = &(Map[y][x]); }
	inline void SetOnwer(Enemy* owner) { this->owner = owner; }
	inline void SetTarget(Player* player) { this->target = player; }
	virtual ~AstarManager() {};
};

