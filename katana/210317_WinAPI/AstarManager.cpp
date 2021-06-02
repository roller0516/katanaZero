#include "AstarManager.h"
#include "CommonFunction.h"
#include "Enemy.h"
#include "Camera.h"
#include "Player.h"

HRESULT AstarTile::Init()
{
	return S_OK;
}

HRESULT AstarTile::Init(int idX, int idY)
{
	isInOpenlist = false;
	isClosed = false;

	this->idX = idX;
	this->idY = idY;

	center.x = idX * TILESIZE + (TILESIZE / 2);
	center.y = idY * TILESIZE + (TILESIZE / 2);

	rc.left = idX * TILESIZE;
	rc.right = rc.left + TILESIZE;
	rc.top = idY * TILESIZE;
	rc.bottom = rc.top + TILESIZE;

	costFromStart = 0.0f;
	costToGoal = 0.0f;
	totalCost = 0.0f;

	type = AstarTileType::None;

	parentTile = nullptr;

	//if( type == AstarTileType::End)
	//	color = RGB(100, 100, 100);

	hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

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
	//hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	//Rectangle(hdc, rc.left - Camera::GetSingleton()->GetCameraPos().x, rc.top - Camera::GetSingleton()->GetCameraPos().y,
	//	rc.right - Camera::GetSingleton()->GetCameraPos().x, rc.bottom - Camera::GetSingleton()->GetCameraPos().y);
	//SelectObject(hdc, hOldBrush);

	if (parentTile) 
	{
		
	}
	//hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	//Rectangle(hdc, rc.left - Camera::GetSingleton()->GetCameraPos().x, rc.top - Camera::GetSingleton()->GetCameraPos().y,
	//	rc.right - Camera::GetSingleton()->GetCameraPos().x, rc.bottom - Camera::GetSingleton()->GetCameraPos().y);
	//SelectObject(hdc, hOldBrush);
	//if (type == AstarTileType::End)
	//{
	//	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	//	Rectangle(hdc, rc.left - Camera::GetSingleton()->GetCameraPos().x, rc.top - Camera::GetSingleton()->GetCameraPos().y,
	//	rc.right - Camera::GetSingleton()->GetCameraPos().x, rc.bottom - Camera::GetSingleton()->GetCameraPos().y);
	//	SelectObject(hdc, hOldBrush);
	//}
}

void AstarTile::Clear()
{
	isInOpenlist = false;
	isClosed = false;

	this->idX = idX;
	this->idY = idY;

	center.x = idX * TILESIZE + (TILESIZE / 2);
	center.y = idY * TILESIZE + (TILESIZE / 2);

	rc.left = idX * TILESIZE;
	rc.right = rc.left + TILESIZE;
	rc.top = idY * TILESIZE;
	rc.bottom = rc.top + TILESIZE;

	costFromStart = 0.0f;
	costToGoal = 0.0f;
	totalCost = 0.0f;

	color = RGB(100, 100, 100);

	hBrush = (HBRUSH)GetStockObject(NULL_BRUSH); 

	parentTile = nullptr;
}

void AstarTile::SetColor(COLORREF color,bool nullcolor)
{
	this->color = color;
	this->hBrush = CreateSolidBrush(color);
	DeleteObject(hBrush);
}

HRESULT AstarManager::Init()
{
	for (int i = 0; i < TILE_X; i++)	// 세로반복 (y)
	{
		for (int j = 0; j < TILE_Y; j++)	// 가로반복 (x)
		{
			Map[i][j].Init(j, i);
		}
	}
	int worldX = Camera::GetSingleton()->GetWorld().x;
	int worldY = Camera::GetSingleton()->GetWorld().y;
	rcMain = { 0,0,worldX,worldY};
	angle = 0;
	size = 100;
	isFind = true;
	return S_OK;
}

void AstarManager::Release()
{
	
}

void AstarManager::Update()
{
	

	if (owner)
	{ 
		if (RectInRect(owner->GetData()->shape, rcMain))
		{
			int posX = 0;
			int	posY = 0;

			if (owner->GetData()->dir == EnemyDir::Right) 
			{
				posX = owner->GetData()->worldPos.x;
				posY = owner->GetData()->worldPos.y + 45;
			}
			else 
			{
				posX = owner->GetData()->worldPos.x;
				posY = owner->GetData()->worldPos.y + 45;
			}
			 
			onwerTileIndex.x = posX / TILESIZE;
			onwerTileIndex.y = posY / TILESIZE;

			int xFrame = onwerTileIndex.x;
			int yFrame = onwerTileIndex.y;

			if (Map[yFrame][xFrame].GetType() == AstarTileType::Wall)
			{
				onwerTileIndex.y -= 1;
				yFrame -= 1;
			}
			
			startTile = &Map[yFrame][xFrame];
			startTile->SetColor(RGB(50, 0, 50), false);
		}
	}
	if (target)
	{ 
		if (RectInRect(target->GetRect(), rcMain) )
		{
			int posX = target->GetWorldpos().x;
			int posY = target->GetWorldpos().y + 35;
			targetTileIndex.x = posX / TILESIZE;
			targetTileIndex.y = posY / TILESIZE;
			int xFrame = targetTileIndex.x;
			int yFrame = targetTileIndex.y;
			if (Map[yFrame][xFrame].GetType() == AstarTileType::Wall)
			{
				yFrame -= 1;
			}
			SetDestTile(xFrame, yFrame);
		}
	}

	if (owner && target)
	{
		if (parentList.size() == 0)
			count = 0;
		if (owner->GetData()->isAttack)
			count = 0;
		if (destTile->GetType() == AstarTileType::Wall)
			Clear();
		if (owner->GetData()->isFind && owner->GetData()->isAttack == false && target->GetisGround() &&
			Map[(int)targetTileIndex.x][(int)targetTileIndex.y].GetType() != AstarTileType::Wall)
		{
			if (startTile != destTile)
			{
				if (count == 0)
				{
					Clear();

					if (startTile)
					{
						currTile = startTile;
					}

					FindPath();
					count++;
				}

			}
		}
	}

	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_SPACE))
	{
		Clear();

		if (isFind && startTile)
		{
			isFind = false;
			currTile = startTile;
		}

		FindPath();
	}
}	

void AstarManager::Render(HDC hdc)
{
	for (int i = 0; i < TILE_Y; i++)	// 세로반복 (y)
	{
		for (int j = 0; j < TILE_X; j++)	// 가로반복 (x)
		{
			if (Map[i][j].GetCenter().x - Camera::GetSingleton()->GetCameraPos().x > WINSIZE_X)
				continue;
			if (Map[i][j].GetCenter().y - Camera::GetSingleton()->GetCameraPos().y > WINSIZE_Y)
				continue;
			Map[i][j].Render(hdc);
		}
	}
}

void AstarManager::Move()
{	
	
}
void AstarManager::FindPath()
{
	//if (heapCount > 3980) 
	//	return;
	if (currTile)
	{
		// 주위에 있는 이동가능한 타일들을 F값 계산 후보에 넣는다.
		AddOpenList(currTile);
		currTile->SetIsClosed(true);
		//DeleteTileInOpenlist(currTile);

		// 후보들 중 F값이 가장 작은 타일을 다음 currTile 선정
		currTile = GetMinTotalCostTileWithHeap();
   		//currTile->SetColor(RGB(130, 200, 130),false);

		// 도착여부 판단
		if (currTile == destTile)
		{
			MarkTileToType(0);
			return;
		} 
		heapCount++;
		// 반복
		FindPath();
	}

}

void AstarManager::AddOpenList(AstarTile* currTile)
{
	int currIdX = currTile->GetIdX();
	int currIdY = currTile->GetIdY();

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)	continue;

			if (currIdY + i < 0 || currIdY + i >= TILE_Y ||
				currIdX + j < 0 || currIdX + j >= TILE_X)
				continue;
				
			// TODO : 오픈리스트에 추가하면 안되는 조건
			// 1. 타입이 wall 일 때
			// 2. 이미 확인된 타일일 때 ( closeList에 있을 때 )
			if (Map[currIdY + i][currIdX + j].GetIsClosed())
				continue;

			if (Map[currIdY + i][currIdX + j].GetType() ==
				AstarTileType::Wall)	continue;

			// 3. 이미 오픈리스트에 있을 때
			// 조건에 따라 다른 처리
			int F, G, H;
			if (Map[currIdY + i][currIdX + j].GetIsInOpenlist())
			{
				G = Map[currIdY + i][currIdX + j].GetG();
				if (G > currTile->GetG() + CalcEdgeCost(i, j))
				{
					G = currTile->GetG() + CalcEdgeCost(i, j);
					Map[currIdY + i][currIdX + j].SetG(G);

					F = G + Map[currIdY + i][currIdX + j].GetH();
					Map[currIdY + i][currIdX + j].SetF(F);

					// parentNode도 함께 갱신한다.
					Map[currIdY + i][currIdX + j].SetParentTile(currTile);
				}
			}
			else
			{
				// F, G, H를 갱신한다.
				// 대각선인지 상하좌우인지 판단
				G = currTile->GetG() + CalcEdgeCost(i, j);
				H = CalcHeuristics(currIdX + j, currIdY + i);
				F = G + H;
				Map[currIdY + i][currIdX + j].SetG(G);
				Map[currIdY + i][currIdX + j].SetH(H);
				Map[currIdY + i][currIdX + j].SetF(F);

				// parentNode도 함께 갱신한다.
				Map[currIdY + i][currIdX + j].SetParentTile(currTile);

				// openList에 추가
				Map[currIdY + i][currIdX + j].SetIsInOpenlist(true);
				//openList.push_back(&(Map[currIdY + i][currIdX + j]));

				// heap 자료구조에 추가
				InsertOpenlistWithHeap(&(Map[currIdY + i][currIdX + j]));
			}

			//Map[currIdY + i][currIdX + j].SetColor(RGB(205, 130, 130),false);
		}
	}
}

AstarTile* AstarManager::GetMinTotalCostTile()
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

AstarTile* AstarManager::GetMinTotalCostTileWithHeap()
{
	if (heap.empty())
		return nullptr;
	if (heap.size() > 300)
		return nullptr;
	AstarTile* tile = heap[0];

	heap[0] = heap.back();
	heap[0]->SetHeapIndex(0);
	heap.pop_back();

	// 자식과 비교하면서 정렬한다
	if(heap.size()>0)
 	UpdateLower(heap[0]);

	return tile;
}

void AstarManager::UpdateLower(AstarTile* tile)	// 인덱스 : 3
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

void AstarManager::Swap(AstarTile* tile1, AstarTile* tile2)
{
 	heap[tile1->GetHeapIndex()] = tile2;
	heap[tile2->GetHeapIndex()] = tile1;

	int temp = tile1->GetHeapIndex();

	tile1->SetHeapIndex(tile2->GetHeapIndex());
	tile2->SetHeapIndex(temp);
}

void AstarManager::InsertOpenlistWithHeap(AstarTile* tile)
{
	tile->SetHeapIndex(heap.size());

	heap.push_back(tile);

	// 부모 타일과 비교하면서 갱신
	UpdateUpper(tile);
}

void AstarManager::UpdateUpper(AstarTile* tile)
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

void AstarManager::AddAtsar(int index)
{
	//vector<AstarTile*> v;
	
}

void AstarManager::DeleteTileInOpenlist(AstarTile* tile)
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

int AstarManager::CalcEdgeCost(int x, int y)
{
	int edgeCost = 14;
	int temp = abs(x + y);
	if (temp == 1)
	{
		edgeCost = 10;
	}
	return edgeCost;
}

int AstarManager::CalcHeuristics(int x, int y)
{
	int distX = destTile->GetIdX() - x;
	int distY = destTile->GetIdY() - y;

	return sqrt(distX * distX + distY * distY) * 10;
}

void AstarManager::ParentPopBack(int index)
{
	parentList.pop_back();
}

void AstarManager::Clear()
{
	for (int i = 0; i < TILE_X; i++)	// 세로반복 (y)
	{
		for (int j = 0; j < TILE_Y; j++)	// 가로반복 (x)
		{
			Map[i][j].Clear();
		}
	}
	//backTile = nullptr;
	openList.clear();
	closeList.clear();
	heap.clear();
	parentList.clear();
	isFind = true;
}

void AstarManager::SetDestTile(int x, int y)
{
	 destTile = &(Map[y][x]); 
	 destTile->SetColor(RGB(0, 0, 0), false); 
}

void AstarManager::MarkTileToType(int index)
{
	AstarTile* parentTile = destTile;
	while (parentTile->GetParentTile())
	{
		parentTile->SetColor(RGB(50, 50, 50), false);
		parentTile = parentTile->GetParentTile();
		parentList.push_back(parentTile);
	}
	
	backTile = parentList.back();
	//ParentPopBack(index);
	//parentList.insert(make_pair(index, *GetParentList(index)));
}















