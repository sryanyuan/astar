#ifndef _INC_ASTAR_
#define _INC_ASTAR_
//////////////////////////////////////////////////////////////////////////
/*
	This module is deprecated , please use the AStarPathFinder
*/
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <windows.h>
#endif

#include <set>
#include <vector>
#include <list>
#include <map>
#include "min_heap.h"
//////////////////////////////////////////////////////////////////////////
//	accept the result points
typedef void (*AcceptPathResult)(int, int);
//	provide basic node info to find path , user should provide flag
typedef int (*ProvideNodeBasicInfo)(int, int);

//	flags
enum AstarFlagType
{
	AstarFlag_Invalid = 0,
	AstarFlag_Ok = 1,
};

//	support max coordinate
#define ASTAR_MAX_COORDINATE 0xffffffff

#define ASTAR_MAKELONGLONG(l, h) (((long long)h << 32) | ((long long)l & 0xffffffff))

typedef long long AstarCoordinateKey;

typedef std::set<AstarCoordinateKey> CloseNodes;

struct AstarNode
{
	AstarNode()
	{
		Reset();
	}
	AstarNode(int _nValueG, int _nValueH)
	{
		Reset();
		nValueG = _nValueG;
		nValueH = _nValueH;
	}

	void Reset()
	{
		nPosX = nPoxY = 0;
		nValueG = nValueH = 0;
		pParent = NULL;
		m_bInUse = false;
		bInCloseTable = bInOpenTable = false;
		nHeapIndex = 0;
	}

	int GetValueF()
	{
		return nValueG + nValueH;
	}

	//	member variables
	int nPosX;
	int nPoxY;

	int nValueG;
	int nValueH;

	bool m_bInUse;
	bool bInCloseTable;
	bool bInOpenTable;

	int nHeapIndex;

	AstarNode* pParent;
};

typedef std::vector<AstarNode> OpenNodes;
typedef std::list<AstarNode*> AstarNodeList;
typedef std::map<AstarCoordinateKey, AstarNode*> AstarNodeMap;
typedef std::vector<AstarNode*> AstartNodeVector;
//////////////////////////////////////////////////////////////////////////
class Astar
{
public:
	Astar();
	~Astar();

public:
	virtual int GetValueH(int _nStartX, int _nStartY, int _nDestX, int _nDestY);
	virtual int GetValueG(int _nParentX, int _nParentY, int _nX, int _nY);

	//	please use this function
	bool FindPath(int _nRows, int _nCols, int _nStartX, int _nStartY, int _nDestX, int _nDestY, ProvideNodeBasicInfo _fnProvideNodeBasicInfo, AcceptPathResult _fnAcceptPathResult);

	inline void SetEnableDiagonal(bool _bEnable)
	{
		m_bEnableDiagonal = _bEnable;
	}

protected:
	AstarNode* AddOpenTableElement(int _x, int _y);
	AstarNode* AddOpenTableElementEx(int _x, int _y, int _h, int _g);

	bool _FindPath(int _nStartX, int _nStartY, int _nDestX, int _nDestY, ProvideNodeBasicInfo _fnProvideNodeBasicInfo, AcceptPathResult _fnAcceptPathResult);
	bool _FindPathEx(int _nRows, int _nCols, int _nStartX, int _nStartY, int _nDestX, int _nDestY, ProvideNodeBasicInfo _fnProvideNodeBasicInfo, AcceptPathResult _fnAcceptPathResult);

private:
	//	deprecated, it is slow
	CloseNodes m_xClosedNodes;
	AstarNodeMap m_xOpenTable;
	AstarNodeList m_xOpenTableItems;
	AstarNodeList m_xParentNodes;

	//	using min heap, it is faster
	NodeMinHeap m_xOpenTableHeap;
	AstartNodeVector m_xMapNodes;
	AstarNodeList m_xAllocateNodes;
	int m_nMapRow;
	int m_nMapCol;

	bool m_bEnableDiagonal;
};
//////////////////////////////////////////////////////////////////////////
#endif