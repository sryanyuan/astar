#ifndef _INC_ASTARPATHFINDER_
#define _INC_ASTARPATHFINDER_
//////////////////////////////////////////////////////////////////////////
#include "minheap.h"
#include <list>
//////////////////////////////////////////////////////////////////////////
//	accept the result points
typedef void (*AStarAcceptPathResult)(int, int);
//	provide basic node info to find path , user should provide flag
typedef int (*AStarProvideNodeBasicInfo)(int, int);

//	path node
struct AStarNode
{
	AStarNode()
	{
		Reset();
	}
	AStarNode(int _nValueG, int _nValueH)
	{
		Reset();
		nValueG = _nValueG;
		nValueH = _nValueH;
	}

	void Reset()
	{
		nPosX = nPosY = 0;
		nValueG = nValueH = 0;
		pParent = NULL;
		bInCloseTable = bInOpenTable = false;
	}

	int GetValueF() const
	{
		return nValueG + nValueH;
	}

	//	member variables
	int nPosX;
	int nPosY;

	int nValueG;
	int nValueH;

	bool bInCloseTable;
	bool bInOpenTable;

	AStarNode* pParent;
};

typedef std::list<AStarNode*> AStarNodeList;
typedef std::vector<AStarNode*> AStartNodeVector;

//	flags
enum AStarFlagType
{
	AStarFlag_Invalid = 0,
	AStarFlag_Ok = 1,
};

class AStarPathFinder
{
public:
	AStarPathFinder();
	~AStarPathFinder();

public:
	//	get the h value of 2 coordinate , you can rewrite it 
	virtual int GetValueH(int _nStartX, int _nStartY, int _nDestX, int _nDestY) const;
	//	get the g value of 2 coordinate , you can rewrite it 
	virtual int GetValueG(int _nParentX, int _nParentY, int _nX, int _nY) const;

	//	calculate the path of 2 coordinates , if find the path , it will call the _fnAcceptPathResult and return true , otherwise it will return false
	//	you should provider the coordinate basic info by pass your own _fnProvideNodeBasicInfo function
	bool FindPath(int _nRows, int _nCols, int _nStartX, int _nStartY, int _nDestX, int _nDestY, AStarProvideNodeBasicInfo _fnProvideNodeBasicInfo, AStarAcceptPathResult _fnAcceptPathResult);

protected:
	AStarNode* AddOpenTableElement(int _x, int _y, int _h, int _g);
	bool _FindPath(int _nRows, int _nCols, int _nStartX, int _nStartY, int _nDestX, int _nDestY, AStarProvideNodeBasicInfo _fnProvideNodeBasicInfo, AStarAcceptPathResult _fnAcceptPathResult);

protected:
	//	using min heap, it is faster
	MinHeap<AStarNode*> m_xOpenTableHeap;
	AStartNodeVector m_xMapNodes;
	AStarNodeList m_xAllocateNodes;
	int m_nMapRow;
	int m_nMapCol;
};
//////////////////////////////////////////////////////////////////////////
#endif