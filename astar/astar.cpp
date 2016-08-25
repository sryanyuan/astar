#include "astar.h"
#include <algorithm>
//////////////////////////////////////////////////////////////////////////
static int s_nPosOffsetDiagonal[] = {
	0,1,
	1,1,
	1,0,
	1,-1,
	0,-1,
	-1,-1,
	-1,0,
	-1,1
};

static int s_nValueG[] = {
	10,
	14,
	10,
	14,
	10,
	14,
	10,
	14
};

static int s_nPosOffset[] = {
	0,1,
	1,0,
	-1,0,
	-1,0
}; 

//////////////////////////////////////////////////////////////////////////
Astar::Astar() : m_xOpenNodes(8)
{
	m_bEnableDiagonal = true;
}

Astar::~Astar()
{

}


int Astar::GetValueH(int _nStartX, int _nStartY, int _nDestX, int _nDestY)
{
	int nHValue = abs(_nDestX - _nStartX) + abs(_nDestY - _nStartY);
	return nHValue;
}

int Astar::GetValueG(int _nParentX, int _nParentY, int _nX, int _nY)
{
	if (_nX - _nParentX != 0 &&
		_nY - _nParentY != 0) {
			return 14;
	}
	return 10;
}

AstarNode* Astar::AddOpenTableElement(int _x, int _y)
{
	AstarNode* pNode = new AstarNode;
	pNode->nPosX = _x;
	pNode->nPoxY = _y;
	m_xOpenTable.insert(std::make_pair(ASTAR_MAKELONGLONG(_x, _y), pNode));
	m_xOpenTableItems.push_back(pNode);

	return pNode;
}

bool ValueFGreater(const AstarNode*  _pl, const AstarNode*  _pr)
{	// apply operator> to operands
	return (_pl->nValueG + _pl->nValueH < _pr->nValueG + _pr->nValueH);
}

bool
	Astar::FindPathEx(int _nStartX,
	int _nStartY,
	int _nDestX,
	int _nDestY,
	ProvideNodeBasicInfo _fnProvideNodeBasicInfo,
	AcceptPathResult _fnAcceptPathResult)
{
	//	first, add start node to open table and close table
	AstarNode* pStartNode = AddOpenTableElement(_nStartX, _nStartY);
	pStartNode->nValueH = GetValueH(_nStartX, _nStartY, _nDestX, _nDestY);
	
	m_xClosedNodes.insert(ASTAR_MAKELONGLONG(_nStartX, _nStartY));
	pStartNode->bInCloseTable = true;

	//	now , do the search process
	AstarNode* pSelectNode = NULL;

	while (1) {
		pSelectNode = NULL;

		//	search for the min ValueF node
		m_xOpenTableItems.sort(ValueFGreater);
		printf("%d\n", m_xOpenTableItems.size());

		if (!m_xOpenTableItems.empty()) {
			pSelectNode = m_xOpenTableItems.front();
			m_xOpenTableItems.pop_front();
		}

		if (NULL == pSelectNode) {
			//	can't find the path
			return false;
		}

		//	add to close table
		m_xClosedNodes.insert(ASTAR_MAKELONGLONG(pSelectNode->nPosX, pSelectNode->nPoxY));

		//	search for the around to initialize open list
		bool bInsert = false;
		for (int i = 0; i < 8; i++) {
			int nNextX = pSelectNode->nPosX + s_nPosOffsetDiagonal[i * 2];
			int nNextY = pSelectNode->nPoxY + s_nPosOffsetDiagonal[i * 2 + 1];

			//	is destination ?
			if (nNextX == _nDestX &&
				nNextY == _nDestY) {
					//	get the path !
					_fnAcceptPathResult(nNextX, nNextY);

					while (NULL != pSelectNode) {
						_fnAcceptPathResult(pSelectNode->nPosX, pSelectNode->nPoxY);
						pSelectNode = pSelectNode->pParent;
					}

					return true;
			}

			//	in close table ?
			if (m_xClosedNodes.find(ASTAR_MAKELONGLONG(nNextX, nNextY)) != m_xClosedNodes.end()) {
				continue;
			}

			int nFlag = _fnProvideNodeBasicInfo(nNextX, nNextY);

			if (AstarFlag_Ok != nFlag) {
				//	nothing
				continue;
			}

			//	in open table ?
			AstarNodeMap::iterator openIter = m_xOpenTable.find(ASTAR_MAKELONGLONG(nNextX, nNextY));
			if(openIter != m_xOpenTable.end()) {
				//	check G value again
				AstarNode* pOpenedNode = openIter->second;

				if (pSelectNode->nValueG + s_nValueG[i] < pOpenedNode->nValueG) {
					pOpenedNode->nValueG = pSelectNode->nValueG + s_nValueG[i];
					pOpenedNode->pParent = pSelectNode;
				}

				continue;
			}

			AstarNode* pNode = AddOpenTableElement(nNextX, nNextY);
			pNode->pParent = pSelectNode;
			pNode->nValueH = GetValueH(nNextX, nNextY, _nDestX, _nDestY);
			pNode->nValueG = pSelectNode->nValueG + GetValueG(pSelectNode->nPosX, pSelectNode->nPoxY, nNextX, nNextY);
		}
	}
}