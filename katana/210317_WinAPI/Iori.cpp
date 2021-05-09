#include "Iori.h"
#include "Image.h"

HRESULT Iori::Init()
{
	frame = 0;
	elapsedTime = 0;

	pos.x = WINSIZE_X / 2;
	pos.y = WINSIZE_Y - 200;

	img = new Image();
	if (FAILED(img->Init("Image/Iori_walk.bmp", 612, 104, 9, 1, true, RGB(255, 0, 255))))
	{
		MessageBox(g_hWnd, "Image/Iori_walk.bmp 로드 실패", "경고", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void Iori::Release()
{
	if (img)
	{
		img->Release();
		delete img;
		img = nullptr;
	}
}

void Iori::Update()
{
	elapsedTime++;
	if (elapsedTime >= 10)
	//if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_SPACE))
	{
		frame++;
		if (frame >= 9)
		{
			frame = 0;
		}
		elapsedTime = 0;
		pos.x += 10;
	}
}

void Iori::Render(HDC hdc)
{
	if (img)
	{
		img->FrameRender(hdc, pos.x, pos.y, frame, 0);
	}
}
