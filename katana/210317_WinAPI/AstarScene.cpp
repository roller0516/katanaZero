#include "AstarScene.h"
#include "CommonFunction.h"

HRESULT AstarTile::Init()
{
	return E_NOTIMPL;
}

HRESULT AstarTile::Init(int idX, int idY)
{
	isInOpenlist = false;
	isClosed = false;

	this->idX = idX;
	this->idY = idY;

	center.x = idX * ASTAR_TILE_SIZE + (ASTAR_TILE_SIZE / 2);
	center.y = idY * ASTAR_TILE_SIZE + (ASTAR_TILE_SIZE / 2);

	rc.left = idX * ASTAR_TILE_SIZE;
	rc.right = rc.left + ASTAR_TILE_SIZE;
	rc.top = idY * ASTAR_TILE_SIZE;
	rc.bottom = rc.top + ASTAR_TILE_SIZE;

	costFromStart = 0.0f;
	costToGoal = 0.0f;
	totalCost = 0.0f;

	type = AstarTileType::None;

	parentTile = nullptr;

	color = RGB(100, 100, 100);
	hBrush = CreateSolidBrush(color);

	return S_OK;
}

void AstarTile::Release()
{
	DeleteObject(hBrush);
}

void AstarTile::Update()
{
}

void AstarTile::Render(HDC hdc)
{
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	FillRect(hdc, &rc, hBrush);
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	SelectObject(hdc, hOldBrush);

	if (type != AstarTileType::Wall)
	{
		// F, G, H 출력
		wsprintf(szText, "F : %d", totalCost);
		TextOut(hdc, rc.left + 5, rc.top + 5, szText, strlen(szText));

		wsprintf(szText, "G : %d", costFromStart);
		TextOut(hdc, rc.left + 5, rc.top + 25, szText, strlen(szText));

		wsprintf(szText, "H : %d", costToGoal);
		TextOut(hdc, rc.right - 55, rc.top + 45, szText, strlen(szText));
	}
}

void AstarTile::SetColor(COLORREF color)
{
	this->color = color;
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(color);
}

HRESULT AstarScene::Init()
{
	SetClientRect(g_hWnd, ASTARSIZE_X, ASTARSIZE_Y);

	for (int i = 0; i < ASTAR_TILE_COUNT; i++)	// 세로반복 (y)
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// 가로반복 (x)
		{
			map[i][j].Init(j, i);
		}
	}

	startTile = &(map[1][1]);
	startTile->SetColor(RGB(255, 0, 0));
	startTile->SetType(AstarTileType::Start);

	currTile = startTile;

	destTile = &(map[ASTAR_TILE_COUNT - 2][ASTAR_TILE_COUNT - 2]);
	destTile->SetColor(RGB(0, 0, 255));
	destTile->SetType(AstarTileType::End);
	
	pos.x = startTile->GetCenter().x;
	pos.y = startTile->GetCenter().y;
	angle = 0;
	size = 100;
	rcMain = { 0,0, ASTARSIZE_X ,ASTARSIZE_Y };
	return S_OK;
}

void AstarScene::Release()
{
}

void AstarScene::Update()
{
	rctest.left = pos.x - size / 2;
	rctest.top = pos.y - size / 2;
	rctest.right = pos.x + size / 2;
	rctest.bottom = pos.y + size / 2;

	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
	{
		// g_ptMouse로 인덱스를 계산
		int x, y;
		x = g_ptMouse.x / ASTAR_TILE_SIZE;
		y = g_ptMouse.y / ASTAR_TILE_SIZE;

		if (0 <= x && x < ASTAR_TILE_COUNT &&
			0 <= y && y < ASTAR_TILE_COUNT)
		{
			// 시작이나 목적지가 아닐 때
			if (map[y][x].GetType() != AstarTileType::Start &&
				map[y][x].GetType() != AstarTileType::End)
			{
				map[y][x].SetColor(RGB(50, 50, 50));
				map[y][x].SetType(AstarTileType::Wall);
			}
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_SPACE))
	{
		FindPath();
		AstarTile* parentTile = destTile;
		while (parentTile->GetParentTile())
		{
			parentTile->SetColor(RGB(255, 0, 255));
			parentTile = parentTile->GetParentTile();
			parentList.push_back(parentTile);
		}
	}
	if (RectInRect(rcMain, rctest))
	{
		int posX = pos.x - rcMain.left;
		int posY = pos.y - rcMain.top;
		ptStartSelectedFrame.x = posX / ASTAR_TILE_SIZE;
		ptStartSelectedFrame.y = posY / ASTAR_TILE_SIZE;
	}

	if (parentList.size() > 0)
	{
		removeTile = parentList.back();
		move = true;
	}
	if(move)
		Move();
}

void AstarScene::Render(HDC hdc)
{
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)	// 세로반복 (y)
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// 가로반복 (x)
		{
			map[i][j].Render(hdc);
		}
	}
	Rectangle(hdc, rctest.left, rctest.top, rctest.right, rctest.bottom);
}

void AstarScene::Move() 
{	
	if (removeTile->GetIdX() == ptStartSelectedFrame.x && removeTile->GetIdY() == ptStartSelectedFrame.y) 
	{
		if (parentList.size() == 0) 
		{
			removeTile = destTile;
		}
		else 
		{
			parentList.pop_back();
			if(parentList.size()>0)
				removeTile = parentList.back();
		}
		angle = GetAngle(pos, removeTile->GetCenter());
	}
	pos.x += cosf(angle) * 100 * TimerManager::GetSingleton()->GetElapsedTime();
	pos.y -= sinf(angle) * 100 * TimerManager::GetSingleton()->GetElapsedTime();
}
void AstarScene::FindPath()
{
	if (currTile)
	{
		// 주위에 있는 이동가능한 타일들을 F값 계산 후보에 넣는다.
		AddOpenList(currTile);
		currTile->SetIsClosed(true);
		//DeleteTileInOpenlist(currTile);

		// 후보들 중 F값이 가장 작은 타일을 다음 currTile 선정
		currTile = GetMinTotalCostTileWithHeap();
		currTile->SetColor(RGB(130, 200, 130));

		// 도착여부 판단
		if (currTile == destTile)
		{
			MarkTileToType();

			return;
		}

		// 반복
		FindPath();
	}
}

void AstarScene::AddOpenList(AstarTile* currTile)
{
	int currIdX = currTile->GetIdX();
	int currIdY = currTile->GetIdY();

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)	continue;

			if (currIdY + i < 0 || currIdY + i >= ASTAR_TILE_COUNT ||
				currIdX + j < 0 || currIdX + j >= ASTAR_TILE_COUNT)
				continue;

			// TODO : 오픈리스트에 추가하면 안되는 조건
			// 1. 타입이 wall 일 때
			// 2. 이미 확인된 타일일 때 ( closeList에 있을 때 )
			if (map[currIdY + i][currIdX + j].GetIsClosed())
				continue;

			if (map[currIdY + i][currIdX + j].GetType() ==
				AstarTileType::Wall)	continue;

			// 3. 이미 오픈리스트에 있을 때
			// 조건에 따라 다른 처리
			int F, G, H;
			if (map[currIdY + i][currIdX + j].GetIsInOpenlist())
			{
				G = map[currIdY + i][currIdX + j].GetG();
				if (G > currTile->GetG() + CalcEdgeCost(i, j))
				{
					G = currTile->GetG() + CalcEdgeCost(i, j);
					map[currIdY + i][currIdX + j].SetG(G);

					F = G + map[currIdY + i][currIdX + j].GetH();
					map[currIdY + i][currIdX + j].SetF(F);

					// parentNode도 함께 갱신한다.
					map[currIdY + i][currIdX + j].SetParentTile(currTile);
				}
			}
			else
			{
				// F, G, H를 갱신한다.
				// 대각선인지 상하좌우인지 판단
				G = currTile->GetG() + CalcEdgeCost(i, j);
				H = CalcHeuristics(currIdX + j, currIdY + i);
				F = G + H;
				map[currIdY + i][currIdX + j].SetG(G);
				map[currIdY + i][currIdX + j].SetH(H);
				map[currIdY + i][currIdX + j].SetF(F);

				// parentNode도 함께 갱신한다.
				map[currIdY + i][currIdX + j].SetParentTile(currTile);

				// openList에 추가
				map[currIdY + i][currIdX + j].SetIsInOpenlist(true);
				//openList.push_back(&(map[currIdY + i][currIdX + j]));

				// heap 자료구조에 추가
				InsertOpenlistWithHeap(&(map[currIdY + i][currIdX + j]));
			}

			map[currIdY + i][currIdX + j].SetColor(RGB(205, 130, 130));
		}
	}
}

AstarTile* AstarScene::GetMinTotalCostTile()
{
	// TODO : heap 정렬을 통한 효율성 증대
	int minF = INT_MAX;
	AstarTile* minFTile = nullptr;
	for (int i = 0; i < openList.size(); i++)
	{
		if (minF > openList[i]->GetF())
		{
			minF = openList[i]->GetF();
			minFTile = openList[i];
		}
	}

	return minFTile;
}

AstarTile* AstarScene::GetMinTotalCostTileWithHeap()
{
	if (heap.empty()) return nullptr;

	AstarTile* tile = heap[0];

	heap[0] = heap.back();
	heap[0]->SetHeapIndex(0);
	heap.pop_back();

	// 자식과 비교하면서 정렬한다
	UpdateLower(heap[0]);

	return tile;
}

void AstarScene::UpdateLower(AstarTile* tile)	// 인덱스 : 3
{
	int lChildIndex = tile->GetHeapIndex() * 2 + 1;

	if (lChildIndex >= heap.size())
		return;

	AstarTile* lChild = heap[lChildIndex];	// 부모의 인덱스 * 2

	AstarTile* minChild = lChild;
	int rChildIndex = tile->GetHeapIndex() * 2 + 2;
	if (rChildIndex < heap.size())
	{
		AstarTile* rChild = heap[rChildIndex];	// 부모의 인덱스 * 2 + 1
		// 비교
		if (lChild->GetF() > rChild->GetF())
		{
			minChild = rChild;
		}
	}

	if (tile->GetF() > minChild->GetF())
	{
		Swap(tile, minChild);
		UpdateLower(tile);
	}
}

void AstarScene::Swap(AstarTile* tile1, AstarTile* tile2)
{
	heap[tile1->GetHeapIndex()] = tile2;
	heap[tile2->GetHeapIndex()] = tile1;

	int temp = tile1->GetHeapIndex();
	tile1->SetHeapIndex(tile2->GetHeapIndex());
	tile2->SetHeapIndex(temp);
}

void AstarScene::InsertOpenlistWithHeap(AstarTile* tile)
{
	tile->SetHeapIndex(heap.size());
	heap.push_back(tile);

	// 부모 타일과 비교하면서 갱신
	UpdateUpper(tile);
}

void AstarScene::UpdateUpper(AstarTile* tile)
{
	if (tile->GetHeapIndex() == 0)
		return;

	int parentIndex = (tile->GetHeapIndex() - 1) / 2;
	AstarTile* parentTile = heap[parentIndex];
	if (parentTile->GetF() > tile->GetF())
	{
		Swap(parentTile, tile);
		UpdateUpper(tile);
	}
}

void AstarScene::DeleteTileInOpenlist(AstarTile* tile)
{
	// TODO : heap 구조를 통해서 개선
	vector<AstarTile*>::iterator it;
	for (it = openList.begin(); it != openList.end();)
	{
		if ((*it) == tile)
		{
			it = openList.erase(it);
			break;
		}
		else
		{
			it++;
		}
	}
}

int AstarScene::CalcEdgeCost(int x, int y)
{
	int edgeCost = 14;
	int temp = abs(x + y);
	if (temp == 1)
	{
		edgeCost = 10;
	}
	return edgeCost;
}

int AstarScene::CalcHeuristics(int x, int y)
{
	int distX = destTile->GetIdX() - x;
	int distY = destTile->GetIdY() - y;

	return sqrt(distX * distX + distY * distY) * 10;
}

void AstarScene::MarkTileToType()
{
	AstarTile* parentTile = destTile;
	while (parentTile)
	{
		parentTile->SetColor(RGB(255, 0, 255));
		parentTile = parentTile->GetParentTile();
	}
}
