#include "AstarManager.h"
#include "CommonFunction.h"
#include "Enemy.h"
#include "Camera.h"

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

	color = RGB(100, 100, 100);

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
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, rc.left-Camera::GetSingleton()->GetCameraPos().x, rc.top - Camera::GetSingleton()->GetCameraPos().y,
		rc.right - Camera::GetSingleton()->GetCameraPos().x, rc.bottom - Camera::GetSingleton()->GetCameraPos().y);
	SelectObject(hdc, hOldBrush);
}

void AstarTile::SetColor(COLORREF color,bool nullcolor)
{
 	this->color = color;
	DeleteObject(hBrush);
	this->hBrush = CreateSolidBrush(color);
}

HRESULT AstarManager::Init()
{
	for (int i = 0; i < TILE_X; i++)	// ���ιݺ� (y)
	{
		for (int j = 0; j < TILE_Y; j++)	// ���ιݺ� (x)
		{
			map[i][j].Init(j, i);
		}
	}

	angle = 0;
	size = 100;
	return S_OK;
}

void AstarManager::Release()
{
	
}

void AstarManager::Update()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_SPACE))
	{
		FindPath();
		AstarTile* parentTile = destTile;
		while (parentTile->GetParentTile())
		{
			parentTile->SetColor(RGB(0, 0, 0),false);
			parentTile = parentTile->GetParentTile();
			parentList.push_back(parentTile);
		}
	}
	if (target) 
	{ 
		if (RectInRect(target->GetRect(), rcMain))
		{
			int posX = target->GetPos().x - 20;
			int posY = target->GetPos().y + 22;
			TileIndex.x = posX / TILESIZE;
			TileIndex.y = posY / TILESIZE;
			int xFrame = TileIndex.x;
			int yFrame = TileIndex.y;
			startTile = &map[yFrame][xFrame];
			startTile->SetColor(RGB(255, 255, 255),false);
			currTile = startTile;
			if (currTile == destTile) 
			{
				destTile = currTile;
				Clear();
			}
		}
	}
	if (parentList.size() > 0)
		backTile = parentList.back();
	
}

void AstarManager::Render(HDC hdc)
{
	for (int i = 0; i < TILE_Y; i++)	// ���ιݺ� (y)
	{
		for (int j = 0; j < TILE_X; j++)	// ���ιݺ� (x)
		{
			if (map[i][j].GetCenter().x - Camera::GetSingleton()->GetCameraPos().x > WINSIZE_X)
				continue;
			if (map[i][j].GetCenter().y - Camera::GetSingleton()->GetCameraPos().y > WINSIZE_Y)
				continue;
			map[i][j].Render(hdc);
		}
	}
}

void AstarManager::Move()
{	
	
}
void AstarManager::FindPath()
{
	if (currTile)
	{
		// ������ �ִ� �̵������� Ÿ�ϵ��� F�� ��� �ĺ��� �ִ´�.
		AddOpenList(currTile);
		currTile->SetIsClosed(true);
		//DeleteTileInOpenlist(currTile);

		// �ĺ��� �� F���� ���� ���� Ÿ���� ���� currTile ����
		currTile = GetMinTotalCostTileWithHeap();
		//currTile->SetColor(RGB(130, 200, 130),false);

		// �������� �Ǵ�
		if (currTile == destTile)
		{
			MarkTileToType();

			return;
		} 

		// �ݺ�
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
				
			// TODO : ���¸���Ʈ�� �߰��ϸ� �ȵǴ� ����
			// 1. Ÿ���� wall �� ��
			// 2. �̹� Ȯ�ε� Ÿ���� �� ( closeList�� ���� �� )
			if (map[currIdY + i][currIdX + j].GetIsClosed())
				continue;

			if (map[currIdY + i][currIdX + j].GetType() ==
				AstarTileType::Wall)	continue;

			// 3. �̹� ���¸���Ʈ�� ���� ��
			// ���ǿ� ���� �ٸ� ó��
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

					// parentNode�� �Բ� �����Ѵ�.
					map[currIdY + i][currIdX + j].SetParentTile(currTile);
				}
			}
			else
			{
				// F, G, H�� �����Ѵ�.
				// �밢������ �����¿����� �Ǵ�
				G = currTile->GetG() + CalcEdgeCost(i, j);
				H = CalcHeuristics(currIdX + j, currIdY + i);
				F = G + H;
				map[currIdY + i][currIdX + j].SetG(G);
				map[currIdY + i][currIdX + j].SetH(H);
				map[currIdY + i][currIdX + j].SetF(F);

				// parentNode�� �Բ� �����Ѵ�.
				map[currIdY + i][currIdX + j].SetParentTile(currTile);

				// openList�� �߰�
				map[currIdY + i][currIdX + j].SetIsInOpenlist(true);
				//openList.push_back(&(map[currIdY + i][currIdX + j]));

				// heap �ڷᱸ���� �߰�
				InsertOpenlistWithHeap(&(map[currIdY + i][currIdX + j]));
			}

			//map[currIdY + i][currIdX + j].SetColor(RGB(205, 130, 130),false);
		}
	}
}

AstarTile* AstarManager::GetMinTotalCostTile()
{
	// TODO : heap ������ ���� ȿ���� ����
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
	if (heap.empty()) return nullptr;

	AstarTile* tile = heap[0];

	heap[0] = heap.back();
	heap[0]->SetHeapIndex(0);
	heap.pop_back();

	// �ڽİ� ���ϸ鼭 �����Ѵ�
 	UpdateLower(heap[0]);

	return tile;
}

void AstarManager::UpdateLower(AstarTile* tile)	// �ε��� : 3
{
	int lChildIndex = tile->GetHeapIndex() * 2 + 1;

	if (lChildIndex >= heap.size())
		return;

	AstarTile* lChild = heap[lChildIndex];	// �θ��� �ε��� * 2

	AstarTile* minChild = lChild;
	int rChildIndex = tile->GetHeapIndex() * 2 + 2;
	if (rChildIndex < heap.size())
	{
		AstarTile* rChild = heap[rChildIndex];	// �θ��� �ε��� * 2 + 1
		// ��
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

	// �θ� Ÿ�ϰ� ���ϸ鼭 ����
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

void AstarManager::DeleteTileInOpenlist(AstarTile* tile)
{
	// TODO : heap ������ ���ؼ� ����
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

void AstarManager::ParentPopBack()
{
	parentList.pop_back();
}

void AstarManager::Clear()
{
	for (int i = 0; i < TILE_X; i++)	// ���ιݺ� (y)
	{
		for (int j = 0; j < TILE_Y; j++)	// ���ιݺ� (x)
		{
			map[i][j].Init(j, i);
		}
	}
	backTile = nullptr;
	openList.clear();
	closeList.clear();
	heap.clear();
	parentList.clear();
}

void AstarManager::MarkTileToType()
{
	AstarTile* parentTile = destTile;
	while (parentTile)
	{
		parentTile->SetColor(RGB(255, 0, 255),false);
		parentTile = parentTile->GetParentTile();
	}
}
