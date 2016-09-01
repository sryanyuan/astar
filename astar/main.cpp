// astar.cpp : 定义控制台应用程序的入口点。
//

#include "memleak_detect_fileline.h"

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "stdafx.h"
#include "AStarPathFinder.h"
#include <windows.h>
//////////////////////////////////////////////////////////////////////////
//	18 * 18
const int row = 18;
const int col = 18;

int charmap[row*col] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,1, 1, 0, 1, 0, 0, 0, 0, 0,
	1, 1, 0, 1, 0, 0, 0, 1, 0,1, 1, 0, 1, 0, 0, 0, 1, 0,
	0, 0, 0, 1, 1, 1, 0, 1, 0,1, 1, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,1, 1, 0, 1, 0, 0, 0, 1, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 1,1, 1, 0, 1, 0, 0, 0, 1, 0,
	1, 0, 0, 0, 1, 1, 1, 0, 1,1, 1, 0, 1, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,1, 1, 0, 1, 0, 0, 0, 1, 0,
	1, 1, 1, 0, 0, 0, 0, 0, 1,0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 1, 0, 0, 0, 0,1, 1, 1, 1, 0, 0, 0, 1, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 1,1, 1, 0, 1, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,1, 1, 0, 1, 0, 0, 0, 1, 0,
	1, 1, 0, 1, 0, 0, 0, 0, 1,0, 0, 0, 1, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 1, 0, 0, 0, 0,1, 1, 0, 1, 0, 0, 1, 1, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 1,1, 1, 1, 1, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,0, 1, 0, 1, 0, 0, 0, 1, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 1,1, 1, 0, 0, 0, 1, 0, 1, 0,
	0, 0, 0, 0, 1, 0, 0, 0, 0,1, 1, 0, 1, 0, 0, 0, 1, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 1,1, 1, 0, 1, 0, 0, 0, 1, 0
};

int fnGetBasicInfo(int _x, int _y)
{
	if (_x < 0 ||
		_x >= col) {
		return AStarFlag_Invalid;
	}
	if (_y < 0 ||
		_y >= row) {
		return AStarFlag_Invalid;
	}

	if (charmap[_y * col + _x] == 0 ||
		charmap[_y * col + _x] == 2) {
		return AStarFlag_Ok;
	}

	return AStarFlag_Invalid;
}

void fnAcceptResult(int _x, int _y) {
	charmap[_x + _y * col] = 2;
}

void exampleAtar()
{
	unsigned int tick = GetTickCount();

	AStarPathFinder finder;
	if (!finder.FindPath(row, col, 0, 0, 17, 17, fnGetBasicInfo, fnAcceptResult)) {
		printf("can't find path\n");
		printf("cost %d ms\n", GetTickCount() - tick);
	} else {
		printf("cost %d ms\n", GetTickCount() - tick);
		//	print the map
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				printf("%d ", charmap[i * col + j]);
			}
			printf("\n");
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hModule = LoadLibrary("user32.dll");
	exampleAtar();

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	FreeLibrary(hModule);
	hModule = 0;

	return 0;
}
