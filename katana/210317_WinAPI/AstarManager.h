#pragma once
#include "GameNode.h"

enum class AstarTileType { Start, End, Wall, None };

class AstarTile : public GameNode
{
private:
	int idX, idY;
	POINT center;
	RECT rc;
	AstarTileType type;

	int costFromStart;	// g : ���������� ���� �������� ���
	int costToGoal;		// h : ���� ������ ������������ ������
	int totalCost;		// f : g + h

	AstarTile* parentTile;	// g�� ���ŵ� ������ ���� ��带 ����

	bool isInOpenlist;
	bool isClosed;

	COLORREF color;
	HBRUSH hBrush;
	HBRUSH hOldBrush;

	char szText[128];

	// heap ����
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
	AstarTile map[TILE_X][TILE_Y];

	AstarTile* startTile;	// ������
	AstarTile* destTile;	// �Ķ���
	AstarTile* backTile;
	AstarTile* currTile;	// (���) �ĺ�Ÿ���� ������ ���� ����Ÿ��

	vector<AstarTile*> openList;
	vector<AstarTile*> closeList;
	vector<AstarTile*> heap;
	vector<AstarTile*> parentList;

	Player* target;
	Enemy* owner;
	FPOINT pos;
	RECT rctest;
	RECT rcMain;

	FPOINT onwerTileIndex;
	FPOINT targetTileIndex;
	int ptSelected[3];
	int size;
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
	// heap ���
	AstarTile* GetMinTotalCostTileWithHeap();
	void UpdateLower(AstarTile* tile);
	void Swap(AstarTile* tile1, AstarTile* tile2);
	void InsertOpenlistWithHeap(AstarTile* tile);
	void UpdateUpper(AstarTile* tile);
	void SetRect(RECT rc) { this->rcMain = rc; }
	void SetWall(int x, int y) { map[x][y].SetType(AstarTileType::Wall); }

	void DeleteTileInOpenlist(AstarTile* tile);
	void MarkTileToType();
	vector<AstarTile*> GetParentList() { return this->parentList; }
	int CalcEdgeCost(int x, int y);
	int CalcHeuristics(int x, int y);
	void ParentPopBack();
	void Clear();

	inline AstarTile* GetDestTile() { return this->destTile; }
	inline void SetBackTile(AstarTile* astar) { this->backTile = astar; }
	inline FPOINT GetTileIndex() { return this->onwerTileIndex; }
	inline AstarTile* GetBackTile() {return this->backTile;}
	inline RECT GetMainRect() { return this->rcMain; }
	inline void SetDestTile(int x, int y) { destTile = &(map[y][x]); destTile->SetColor(RGB(0, 0, 0), false);}
	inline void SetStartTile(int x, int y) { startTile = &(map[y][x]); }
	inline void SetOnwer(Enemy* owner) { this->owner = owner; }
	inline void SetTarget(Player* player) { this->target = player; }
	virtual ~AstarManager() {};
};
