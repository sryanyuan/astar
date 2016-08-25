#ifndef _INC_ASTAR_
#define _INC_ASTAR_
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <windows.h>
#endif

#include <set>
#include <vector>
#include <list>
#include <map>
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

	void Reset()
	{
		nPosX = nPoxY = 0;
		nValueG = nValueH = 0;
		pParent = NULL;
		m_bInUse = false;
		bInCloseTable = bInOpenTable = false;
	}

	//	member variables
	int nPosX;
	int nPoxY;

	int nValueG;
	int nValueH;

	bool m_bInUse;
	bool bInCloseTable;
	bool bInOpenTable;

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

	bool FindPathEx(int _nStartX, int _nStartY, int _nDestX, int _nDestY, ProvideNodeBasicInfo _fnProvideNodeBasicInfo, AcceptPathResult _fnAcceptPathResult);

	inline void SetEnableDiagonal(bool _bEnable)
	{
		m_bEnableDiagonal = _bEnable;
	}

protected:
	AstarNode* AddOpenTableElement(int _x, int _y);

private:
	CloseNodes m_xClosedNodes;
	OpenNodes m_xOpenNodes;
	AstarNodeMap m_xOpenTable;
	AstarNodeList m_xOpenTableItems;
	AstarNodeList m_xParentNodes;
	bool m_bEnableDiagonal;
};
//////////////////////////////////////////////////////////////////////////
#endif