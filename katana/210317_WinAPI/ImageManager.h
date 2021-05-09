#pragma once
#include "config.h"
#include "Singleton.h"

class Image;
class ImageManager : public Singleton<ImageManager>
{
private:
	map<string, Image*> mImageDatas;
public:
	HRESULT Init();
	void Release();

	Image* AddImage(string key, const char* fileName, 
		int width, int height, bool isTransparent = FALSE, 
		COLORREF transColor = FALSE);

	Image* AddImage(string key, const char* fileName,
		int width, int height, int maxFrameX, int maxFrameY,
		bool isTransparent = FALSE, COLORREF transColor = FALSE);

	void DeleteImage(string key);
	Image* FindImage(string key);

};

