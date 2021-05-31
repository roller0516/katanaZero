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

    blendFunc.BlendOp = AC_SRC_OVER;
    blendFunc.BlendFlags = 0;
    blendFunc.AlphaFormat = 0;

    imageInfo->hBlendDC = CreateCompatibleDC(hdc);
    imageInfo->hBlendBit = CreateCompatibleBitmap(hdc,
        imageInfo->width, imageInfo->height);
    imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC,
        imageInfo->hBlendBit);

    imageInfo->blendWidth= WINSIZE_X;
    imageInfo->blendheight = WINSIZE_Y;

    imageInfo->maxFrameX = maxFrameX;
    imageInfo->maxFrameY = maxFrameY;
    imageInfo->frameWidth = width / maxFrameX;
    imageInfo->frameHeight = height / maxFrameY;
    imageInfo->currFrameX = 0;
    imageInfo->currFrameY = 0;

    int size;
    imageInfo->frameWidth > imageInfo->frameHeight ? size = imageInfo->frameWidth : size = imageInfo->frameHeight;

    imageInfo->hRotateDC = CreateCompatibleDC(hdc);
    imageInfo->hRotateBitmap = CreateCompatibleBitmap(hdc, size, size);
    imageInfo->hOldRotateBit =
        (HBITMAP)SelectObject(imageInfo->hRotateDC, imageInfo->hRotateBitmap);

    imageInfo->rotateWidth = size;
    imageInfo->rotateHeight = size;
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

void Image::rotateRender(HDC hdc, float destX, float destY, int currentFrameX, int currentFrameY, float angle, float size)
{
    POINT rPoint[3];
    int dist = sqrt((imageInfo->frameWidth / 2) * (imageInfo->frameWidth / 2) + (imageInfo->frameHeight / 2) * (imageInfo->frameHeight / 2));
    float baseAngle[3];
    baseAngle[0] = PI - atanf(((float)imageInfo->frameHeight / 2) / ((float)imageInfo->frameWidth / 2));
    baseAngle[1] = atanf(((float)imageInfo->frameHeight / 2) / ((float)imageInfo->frameWidth / 2));
    baseAngle[2] = PI + atanf(((float)imageInfo->frameHeight / 2) / ((float)imageInfo->frameWidth / 2));

    for (int i = 0; i < 3; ++i)
    {
        rPoint[i].x = (imageInfo->rotateWidth / 2 + cosf(baseAngle[i] + angle) * dist);
        rPoint[i].y = (imageInfo->rotateHeight / 2 + -sinf(baseAngle[i] + angle) * dist);
    }

    if (isTransparent)
    {
        BitBlt(imageInfo->hRotateDC, 0, 0,
            imageInfo->rotateWidth, imageInfo->rotateHeight,
            hdc, 0, 0, BLACKNESS);

        HBRUSH hBrush = CreateSolidBrush(transColor);
        HBRUSH oBrush = (HBRUSH)SelectObject(imageInfo->hRotateDC, hBrush);
        ExtFloodFill(imageInfo->hRotateDC, 1, 1, RGB(0, 0, 0), FLOODFILLSURFACE);
        DeleteObject(hBrush);

        PlgBlt(imageInfo->hRotateDC, rPoint, imageInfo->hMemDC,
            currentFrameX * imageInfo->frameWidth,
            currentFrameY * imageInfo->frameHeight,
            imageInfo->frameWidth, imageInfo->frameHeight, NULL, 0, 0);

        GdiTransparentBlt(hdc,
            destX - imageInfo->rotateWidth / 2 * size,
            destY - imageInfo->rotateHeight / 2 * size,
            imageInfo->rotateWidth*size,
            imageInfo->rotateHeight*size,
            imageInfo->hRotateDC,
            0,
            0,
            imageInfo->rotateWidth,
            imageInfo->rotateHeight,
            transColor);
    }
    else
    {
        PlgBlt(hdc, rPoint, imageInfo->hMemDC,
            currentFrameX * imageInfo->frameWidth,
            currentFrameY * imageInfo->frameHeight,
            imageInfo->frameWidth, imageInfo->frameHeight, NULL, 0, 0);
    }

}

void Image::rotateRenderFlip(HDC hdc, float destX, float destY, int currentFrameX, int currentFrameY, float angle, float size )
{
    POINT rPoint[3];
    int dist = sqrt((imageInfo->frameWidth / 2) * (imageInfo->frameWidth / 2) + (imageInfo->frameHeight / 2) * (imageInfo->frameHeight / 2));
    float baseAngle[3];
    baseAngle[0] = PI - atanf(((float)imageInfo->frameHeight / 2) / ((float)imageInfo->frameWidth / 2));
    baseAngle[1] = atanf(((float)imageInfo->frameHeight / 2) / ((float)imageInfo->frameWidth / 2));
    baseAngle[2] = PI + atanf(((float)imageInfo->frameHeight / 2) / ((float)imageInfo->frameWidth / 2));

    for (int i = 0; i < 3; ++i)
    {
        rPoint[i].x = (imageInfo->rotateWidth / 2 + cosf(baseAngle[i] + angle) * dist);
        rPoint[i].y = (imageInfo->rotateHeight / 2 + -sinf(baseAngle[i] + angle) * dist);
    }

    if (isTransparent)
    {
        BitBlt(imageInfo->hRotateDC, 0, 0,
            imageInfo->rotateWidth, imageInfo->rotateHeight,
            hdc, 0, 0, BLACKNESS);

        HBRUSH hBrush = CreateSolidBrush(transColor);
        HBRUSH oBrush = (HBRUSH)SelectObject(imageInfo->hRotateDC, hBrush);
        ExtFloodFill(imageInfo->hRotateDC, 1, 1, RGB(0, 0, 0), FLOODFILLSURFACE);
        DeleteObject(hBrush);

        StretchBlt(
            imageInfo->hTempDC,
            0, imageInfo->frameHeight-1,
            imageInfo->frameWidth, -1*imageInfo->frameHeight,
            imageInfo->hMemDC,
            imageInfo->frameWidth * imageInfo->currFrameX,
            imageInfo->frameHeight * imageInfo->currFrameY,
            imageInfo->frameWidth, imageInfo->frameHeight,
            SRCCOPY);

        PlgBlt(imageInfo->hRotateDC, rPoint, imageInfo->hTempDC,
            //_imageInfo->currentFrameX * _imageInfo->frameWidth,
            //_imageInfo->currentFrameY * _imageInfo->frameHeight,
            currentFrameX * imageInfo->frameWidth,
            currentFrameY * imageInfo->frameHeight,
            imageInfo->frameWidth, 
            imageInfo->frameHeight, NULL, 0, 0);

        GdiTransparentBlt(hdc,
            destX - imageInfo->rotateWidth / 2 * size,
            destY - imageInfo->rotateHeight / 2 * size,
            imageInfo->rotateWidth * size,
            imageInfo->rotateHeight * size,
            imageInfo->hRotateDC,
            0,
            0,
            imageInfo->rotateWidth,
            imageInfo->rotateHeight,
            transColor);
    }
    else
    {
        PlgBlt(hdc, rPoint, imageInfo->hMemDC,
            currentFrameX * imageInfo->frameWidth,
            currentFrameY * imageInfo->frameHeight,
            imageInfo->frameWidth, imageInfo->frameHeight, NULL, 0, 0);
    }
}


void Image::FrameRender(HDC hdc, int destX, int destY,
    int currFrameX, int currFrameY, bool isCenterRenderring, float size)
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
            imageInfo->frameWidth-1, 
            imageInfo->frameHeight* imageInfo->currFrameY,
            -1*imageInfo->frameWidth, 
            imageInfo->frameHeight,
            imageInfo->hMemDC,
            imageInfo->frameWidth * imageInfo->currFrameX,
            imageInfo->frameHeight * imageInfo->currFrameY,
            imageInfo->frameWidth,
            imageInfo->frameHeight,
            SRCCOPY);
        // 특정 색상을 빼고 복사하는 함수
        GdiTransparentBlt(
            hdc,                // 목적지 DC
            x, y,               // 복사 위치
            imageInfo->frameWidth * size,
            imageInfo->frameHeight * size,  // 복사 크기
            imageInfo->hTempDC,  // 원본 DC
            0,  // 복사 X 위치
            imageInfo->frameHeight * imageInfo->currFrameY, // 복사 Y 위치
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

void Image::AlphaRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha, bool isCenterRenderring)
{

    imageInfo->currFrameX = currentFrameX;
    imageInfo->currFrameY = currentFrameY;

    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->frameWidth / 2);
        y = destY - (imageInfo->frameHeight / 2);
    }

    blendFunc.SourceConstantAlpha = alpha;

    if (isTransparent)
    {
      
        BitBlt(imageInfo->hBlendDC, 0, 0, imageInfo->frameWidth, imageInfo->frameHeight, hdc, x, y, SRCCOPY);

        GdiTransparentBlt(
            imageInfo->hBlendDC,	//복사할 장소의 DC
            0,					//복사할 좌표 시작X
            0,					//복사할 좌표 시작Y
            imageInfo->frameWidth,		//복사할 이미지 가로크기
            imageInfo->frameHeight,		//복사할 이미지 세로크기
            imageInfo->hMemDC,		//복사될 대상 DC
            imageInfo->currFrameX * imageInfo->frameWidth, 
            imageInfo->currFrameY * imageInfo->frameHeight,					//복사될 대상의 시작지점
            imageInfo->frameWidth,		//복사 영역 가로크기
            imageInfo->frameHeight,		//복사 영역 세로크기
            transColor);			//복사할때 제외할 색상(일반적으로 마젠타)
        //3
        GdiAlphaBlend(hdc, x, y, imageInfo->frameWidth, imageInfo->frameHeight,
            imageInfo->hBlendDC, 0, 0, imageInfo->frameWidth, imageInfo->frameHeight, blendFunc);
    }
    else//원본 이미지 그대로 출력
    {
        GdiAlphaBlend(hdc, x, y, imageInfo->frameWidth, imageInfo->frameHeight,
            imageInfo->hMemDC, 0, 0, imageInfo->frameWidth, imageInfo->frameHeight, blendFunc);
    }
}

//void Image::AlphaRenderFlip(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha, bool isCenterRenderring)
//{
//
//    imageInfo->currFrameX = currentFrameX;
//    imageInfo->currFrameY = currentFrameY;
//
//    int x = destX;
//    int y = destY;
//    if (isCenterRenderring)
//    {
//        x = destX - (imageInfo->frameWidth / 2);
//        y = destY - (imageInfo->frameHeight / 2);
//    }
//
//    blendFunc.SourceConstantAlpha = alpha;
//
//    if (isTransparent)
//    {
//        BitBlt(imageInfo->hBlendDC, 0, 0, imageInfo->frameWidth, imageInfo->frameHeight, hdc, x, y, SRCCOPY);
//        StretchBlt(
//            imageInfo->hBlendDC,
//            imageInfo->frameWidth - 1,
//            imageInfo->frameHeight * imageInfo->currFrameY,
//            -1 * imageInfo->frameWidth,
//            imageInfo->frameHeight,
//            hdc,
//            x,
//            y,
//            imageInfo->frameWidth,
//            imageInfo->frameHeight,
//            SRCCOPY);
//        // 특정 색상을 빼고 복사하는 함수
//        GdiTransparentBlt(
//            imageInfo->hBlendDC,                // 목적지 DC
//            x, y,               // 복사 위치
//            imageInfo->frameWidth,
//            imageInfo->frameHeight,  // 복사 크기
//            imageInfo->hMemDC,  // 원본 DC
//            0,  // 복사 X 위치
//            imageInfo->frameHeight * imageInfo->currFrameY, // 복사 Y 위치
//            imageInfo->frameWidth,
//            imageInfo->frameHeight,  // 복사 크기
//            transColor  // 제외할 색상
//        );
//        //3
//        GdiAlphaBlend(hdc, x, y, imageInfo->frameWidth, imageInfo->frameHeight,
//            imageInfo->hBlendDC, 0, 0, imageInfo->frameWidth, imageInfo->frameHeight, blendFunc);
//    }
//    else//원본 이미지 그대로 출력
//    {
//        GdiAlphaBlend(hdc, x, y, imageInfo->frameWidth, imageInfo->frameHeight,
//            imageInfo->hMemDC, 0, 0, imageInfo->frameWidth, imageInfo->frameHeight, blendFunc);
//    }
//}

void Image::Release()
{
    if (imageInfo)
    {
        SelectObject(imageInfo->hMemDC, imageInfo->hOldBit);
        DeleteObject(imageInfo->hBitmap);
        DeleteDC(imageInfo->hMemDC);

        SelectObject(imageInfo->hTempDC, imageInfo->hOldTempBit);
        DeleteObject(imageInfo->hTempBitmap);
        DeleteDC(imageInfo->hTempDC);

        SelectObject(imageInfo->hRotateDC, imageInfo->hOldRotateBit);
        DeleteObject(imageInfo->hRotateBitmap);
        DeleteDC(imageInfo->hRotateDC);

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
