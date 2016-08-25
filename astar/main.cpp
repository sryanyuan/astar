// astar.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "astar.h"


//////////////////////////////////////////////////////////////////////////
//	9 * 6
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
	1, 1, 0, 0, 0, 0, 0, 0, 1,1, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,1, 1, 0, 1, 0, 0, 0, 1, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 1,1, 1, 0, 1, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,1, 1, 0, 1, 0, 0, 0, 1, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 1,0, 0, 0, 1, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,1, 1, 0, 1, 0, 0, 0, 1, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 1,1, 1, 0, 1, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,0, 1, 0, 1, 0, 0, 0, 1, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 1,1, 1, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,1, 1, 0, 1, 0, 0, 0, 1, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 1,1, 1, 0, 1, 0, 0, 0, 1, 0
};

/*const int row = 4;
const int col = 4;

int charmap[row*col] = {
	0, 0, 0, 1,
	1, 1, 0, 1,
	0, 0, 0, 1,
	1, 0, 1, 1
};*/

int fnGetBasicInfo(int _x, int _y)
{
	if (_x < 0 ||
		_x >= col) {
		return AstarFlag_Invalid;
	}
	if (_y < 0 ||
		_y >= row) {
		return AstarFlag_Invalid;
	}

	if (charmap[_y * col + _x] == 0) {
		return AstarFlag_Ok;
	}

	return AstarFlag_Invalid;
}

void fnAcceptResult(int _x, int _y) {
	charmap[_x + _y * col] = 2;
}

void exampleAtar()
{
	unsigned int tick = GetTickCount();

	Astar finder;
	if (!finder.FindPathEx(0, 0, 17, 17, fnGetBasicInfo, fnAcceptResult)) {
		printf("can't find path\n");
	}

	printf("cost %d ms\n", GetTickCount() - tick);

	//	print the map
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			printf("%d ", charmap[i * col + j]);
		}
		printf("\n");
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	exampleAtar();

	return 0;
}
