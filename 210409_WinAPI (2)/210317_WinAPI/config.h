// config.h

#pragma once

#include <Windows.h>
#include <string>
#include <ctime>
#include <bitset>
#include <vector>
#include "KeyManager.h"

using namespace std;

#define WINSIZE_X	600
#define WINSIZE_Y	800

typedef struct tagFPoint
{
	float x;
	float y;
} FPOINT, *PFPOINT;

//typedef tagFPoint FPOINT2;
//typedef tagFPoint* PFPOINT2;

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;