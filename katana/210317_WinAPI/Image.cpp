#include "Image.h"

HRESULT Image::Init(int width, int height)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldBit = 
        (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_KIND::EMPTY;

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = FALSE;
    this->transColor = FALSE;

    return S_OK;
}

HRESULT Image::Init(const char* fileName, int width, int height,
    bool isTransparent/* = FALSE*/, COLORREF transColor/* = FALSE*/)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = 
        (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit =
        (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

    // 알파 블랜드
    imageInfo->hBlendDC = CreateCompatibleDC(hdc);
    imageInfo->hBlendBit = CreateCompatibleBitmap(hdc, 
        imageInfo->width, imageInfo->height);
    imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC, 
        imageInfo->hBlendBit);

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    this->blendFunc.AlphaFormat = 0;
    this->blendFunc.BlendFlags = 0;
    this->blendFunc.BlendOp = AC_SRC_OVER;
    this->blendFunc.SourceConstantAlpha = 255;

    return S_OK;
}

HRESULT Image::Init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap =
        (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit =
        (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

    imageInfo->hTempDC = CreateCompatibleDC(hdc);
    imageInfo->hTempBitmap = CreateCompatibleBitmap(hdc, width / maxFrameX, height);
    imageInfo->hOldTempBit =
        (HBITMAP)SelectObject(imageInfo->hTempDC, imageInfo->hTempBitmap);

    imageInfo->hRotateDC = CreateCompatibleDC(hdc);
    imageInfo->hRotateBitmap = CreateCompatibleBitmap(hdc, (width /maxFrameX)*2, height*2);
    imageInfo->hOldRotateBit =
        (HBITMAP)SelectObject(imageInfo->hRotateDC, imageInfo->hRotateBitmap);


    imageInfo->maxFrameX = maxFrameX;
    imageInfo->maxFrameY = maxFrameY;
    imageInfo->frameWidth = width / maxFrameX;
    imageInfo->frameHeight = height / maxFrameY;
    imageInfo->currFrameX = 0;
    imageInfo->currFrameY = 0;

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    return S_OK;
}

void Image::Render(HDC hdc, int destX, int destY, bool isCenterRenderring)
{
    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->width / 2);
        y = destY - (imageInfo->height / 2);
    }

    if (isTransparent)
    {
        // 특정 색상을 빼고 복사하는 함수
        GdiTransparentBlt(
            hdc,
            x, y,
            imageInfo->width, imageInfo->height,

            imageInfo->hMemDC,
            0, 0,
            imageInfo->width, imageInfo->height,
            transColor
        );
    }
    else
    {
        // bitmap 에 있는 이미지 정보를 다른 비트맵에 복사
        BitBlt(
            hdc,                // 복사 목적지 DC
            x, y,               // 복사 시작 위치
            imageInfo->width,   // 원본에서 복사될 가로크기
            imageInfo->height,  // 원본에서 복사될 세로크기
            imageInfo->hMemDC,  // 원본 DC
            0, 0,               // 원본에서 복사 시작 위치
            SRCCOPY             // 복사 옵션
        );     
    }

  
}
void Image::CameraRender(HDC hdc, float destX, float destY, int width, int height, bool isCenterRenderring)
{
    float x = destX;
    float y = destY;
   
    //destY + WINSIZE_Y / 3
    if (isTransparent)
    {

        // 특정 색상을 빼고 복사하는 함수
        GdiTransparentBlt(
            hdc,
            0, 0,
            width, height,
            imageInfo->hMemDC,
            x, y,
            width, height,
            transColor
        );
    }
    else
    {
        // bitmap 에 있는 이미지 정보를 다른 비트맵에 복사
        StretchBlt(
            hdc,                // 복사 목적지 DC
            0, 0,               // 복사 시작 위치
            width,   // 원본에서 복사될 가로크기
            height,  // 원본에서 복사될 세로크기
            imageInfo->hMemDC,  // 원본 DC
            x, y,               // 원본에서 복사 시작 위치
            width, height, SRCCOPY             // 복사 옵션
        );
    }


}

void Image::RotateFrameRender(HDC hdc, int destX, int destY,
    int currFrameX, int currFrameY, bool isCenterRenderring, int size, float angle)
{
    imageInfo->currFrameX = currFrameX;
    imageInfo->currFrameY = currFrameY;

    float cos = cosf(angle * PI / 180);
    float sin = sinf(angle * PI / 180);
   
    XFORM xForm;

    SetGraphicsMode(imageInfo->hRotateDC, GM_ADVANCED);

    int x = destX;
    int y = destY;

    if (isCenterRenderring)
    {
        x = destX - (imageInfo->frameWidth / 2);
        y = destY - (imageInfo->frameHeight / 2);
    }

    xForm.eM11 = cos;
    xForm.eM12 = sin;
    xForm.eM21 = -sin;
    xForm.eM22 = cos;
    xForm.eDx = imageInfo->frameWidth / 2;
    xForm.eDy = imageInfo->frameHeight / 2;

   

    if (isTransparent)
    {
        StretchBlt(
            imageInfo->hRotateDC,
            0, 
            0,
            imageInfo->frameWidth,
            imageInfo->frameHeight,
            imageInfo->hMemDC,
            0,
            0,
            imageInfo->frameWidth,
            imageInfo->frameHeight,
            SRCCOPY);
        SetWorldTransform(imageInfo->hRotateDC, &xForm);
        // 특정 색상을 빼고 복사하는 함수
        GdiTransparentBlt(
            hdc,                // 목적지 DC
            x,
            y,               // 복사 위치
            imageInfo->frameWidth,
            imageInfo->frameHeight,  // 복사 크기

            imageInfo->hRotateDC,  // 원본 DC
            0, // 복사 X 위치
            0, // 복사 Y 위치
            imageInfo->frameWidth, 
            imageInfo->frameHeight,  // 복사 크기
            transColor  // 제외할 색상
        );
    }
    else
    {
        if (size > 1)
        {
            StretchBlt(hdc,
                x, y,
                imageInfo->frameWidth * size,
                imageInfo->frameHeight * size,
                imageInfo->hMemDC,
                imageInfo->frameWidth * imageInfo->currFrameX,
                imageInfo->frameHeight * imageInfo->currFrameY,
                imageInfo->frameWidth,
                imageInfo->frameHeight,
                SRCCOPY);
        }
        else
        {
            BitBlt(
                hdc,
                x, y,
                imageInfo->frameWidth,
                imageInfo->frameHeight,
                imageInfo->hMemDC,
                imageInfo->frameWidth * imageInfo->currFrameX,
                imageInfo->frameHeight * imageInfo->currFrameY,
                SRCCOPY
            );
        }
    }



}

void Image::FrameRender(HDC hdc, int destX, int destY,
    int currFrameX, int currFrameY, bool isCenterRenderring, int size)
{
    imageInfo->currFrameX = currFrameX;
    imageInfo->currFrameY = currFrameY;

    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->frameWidth / 2);
        y = destY - (imageInfo->frameHeight / 2);
    }

    if (isTransparent)
    {
        // 특정 색상을 빼고 복사하는 함수
        GdiTransparentBlt(
            hdc,                // 목적지 DC
            x, y,               // 복사 위치
            imageInfo->frameWidth * size,
            imageInfo->frameHeight * size,  // 복사 크기

            imageInfo->hMemDC,  // 원본 DC
            imageInfo->frameWidth * imageInfo->currFrameX,  // 복사 X 위치
            imageInfo->frameHeight * imageInfo->currFrameY, // 복사 Y 위치
            imageInfo->frameWidth, imageInfo->frameHeight,  // 복사 크기
            transColor  // 제외할 색상
        );
    }
    else
    {
        if (size > 1)
        {
            StretchBlt(hdc,
                x, y,
                imageInfo->frameWidth * size,
                imageInfo->frameHeight * size,
                imageInfo->hMemDC,
                imageInfo->frameWidth * imageInfo->currFrameX,
                imageInfo->frameHeight * imageInfo->currFrameY,
                imageInfo->frameWidth,
                imageInfo->frameHeight,
                SRCCOPY);        
        }       
        else
        {
            BitBlt(
                hdc,
                x, y,
                imageInfo->frameWidth,
                imageInfo->frameHeight,
                imageInfo->hMemDC,
                imageInfo->frameWidth * imageInfo->currFrameX,
                imageInfo->frameHeight * imageInfo->currFrameY,
                SRCCOPY
            );
        }
    }
}

void Image::FrameRenderFlip(HDC hdc, int destX, int destY, int currFrameX, int currFrameY, bool isCenterRenderring, int size)
{
    imageInfo->currFrameX = currFrameX;
    imageInfo->currFrameY = currFrameY;

    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->frameWidth / 2);
        y = destY - (imageInfo->frameHeight / 2);
    }

    if (isTransparent)
    {
        StretchBlt(
            imageInfo->hTempDC,
            imageInfo->frameWidth-1, 0,
            -1*imageInfo->frameWidth, imageInfo->frameHeight,
            imageInfo->hMemDC,
            imageInfo->frameWidth * imageInfo->currFrameX,
            imageInfo->frameHeight * imageInfo->currFrameY,
            imageInfo->frameWidth,imageInfo->frameHeight,
            SRCCOPY);
        // 특정 색상을 빼고 복사하는 함수
        GdiTransparentBlt(
            hdc,                // 목적지 DC
            x, y,               // 복사 위치
            imageInfo->frameWidth * size,
            imageInfo->frameHeight * size,  // 복사 크기
            imageInfo->hTempDC,  // 원본 DC
            0,  // 복사 X 위치
            0, // 복사 Y 위치
            imageInfo->frameWidth, imageInfo->frameHeight,  // 복사 크기
            transColor  // 제외할 색상
        );
        
    }
    else
    {
        if (size > 1)
        {
            StretchBlt(hdc,
                x, y,
                imageInfo->frameWidth * size,
                imageInfo->frameHeight * size,
                imageInfo->hMemDC,
                imageInfo->frameWidth * imageInfo->currFrameX,
                imageInfo->frameHeight * imageInfo->currFrameY,
                imageInfo->frameWidth,
                imageInfo->frameHeight,
                SRCCOPY);
        }
        else
        {
            BitBlt(
                hdc,
                x, y,
                imageInfo->frameWidth,
                imageInfo->frameHeight,
                imageInfo->hMemDC,
                imageInfo->frameWidth * imageInfo->currFrameX,
                imageInfo->frameHeight * imageInfo->currFrameY,
                SRCCOPY
            );
        }
    }
}

void Image::AlphaRender(HDC hdc, int destX, int destY, bool isCenterRenderring)
{
    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->width / 2);
        y = destY - (imageInfo->height / 2);
    }

    // 1. 목적지 DC(hdc)에 그려져 있는 내용을 BlendDC에 복사
    BitBlt(imageInfo->hBlendDC, 0, 0, imageInfo->width, imageInfo->height,
        hdc, x, y, SRCCOPY);

    //GdiTransparentBlt(imageInfo->)

    // 2. 출력할 이미지 DC에 내용을 BlendDC에 지정한 색상을 제외하면서 복사


    // 3.
    AlphaBlend(hdc, x, y, imageInfo->width, imageInfo->height,
        imageInfo->hBlendDC, 0, 0, imageInfo->width, imageInfo->height, blendFunc);
}

void Image::Release()
{
    if (imageInfo)
    {
        SelectObject(imageInfo->hMemDC, imageInfo->hOldBit);
        DeleteObject(imageInfo->hBitmap);
        DeleteDC(imageInfo->hMemDC);

        if (imageInfo->hBlendDC != NULL)
        {
            SelectObject(imageInfo->hBlendDC, imageInfo->hOldBlendBit);
            DeleteObject(imageInfo->hBlendBit);
            DeleteDC(imageInfo->hBlendDC);
        }

        delete imageInfo;
        imageInfo = nullptr;
    }
}
