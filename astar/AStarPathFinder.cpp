#include "AStarPathFinder.h"
#include "memleak_detect_fileline.h"
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
//	min heap compare functions
static bool astarnode_compare_lessorequal(AStarNode* const& _l, AStarNode* const& _r)
{
	return _l->GetValueF() <= _r->GetValueF();
}

//////////////////////////////////////////////////////////////////////////
AStarPathFinder::AStarPathFinder()
{
	m_nMapCol = 0;
	m_nMapRow = 0;
}

AStarPathFinder::~AStarPathFinder()
{

}


int AStarPathFinder::GetValueH(int _nStartX, int _nStartY, int _nDestX, int _nDestY) const
{
	return abs(_nDestX - _nStartX) + abs(_nDestY - _nStartY);
}

int AStarPathFinder::GetValueG(int _nParentX, int _nParentY, int _nX, int _nY) const
{
	if (_nX - _nParentX != 0 &&
		_nY - _nParentY != 0) {
			return 14;
	}
	return 10;
}

AStarNode* AStarPathFinder::AddOpenTableElement(int _x, int _y, int _h, int _g)
{
	AStarNode* pNode = new AStarNode;
	pNode->nPosX = _x;
	pNode->nPosY = _y;
	pNode->nValueH = _h;
	pNode->nValueG = _g;
	m_xOpenTableHeap.Insert(pNode, astarnode_compare_lessorequal);
	m_xMapNodes[_x + _y * m_nMapCol] = pNode;

	m_xAllocateNodes.push_back(pNode);

	return pNode;
}

bool ValueFGreater(const AStarNode*  _pl, const AStarNode*  _pr)
{	// apply operator> to operands
	return _pl->GetValueF() < _pr->GetValueF();
}

bool
	AStarPathFinder::_FindPath(int _nRows,
	int _nCols,
	int _nStartX,
	int _nStartY,
	int _nDestX,
	int _nDestY,
	AStarProvideNodeBasicInfo _fnProvideNodeBasicInfo,
	AStarAcceptPathResult _fnAcceptPathResult)
{
	m_nMapCol = _nCols;
	m_nMapRow = _nRows;
	m_xOpenTableHeap.Resize(size_t(_nRows * _nCols), NULL);
	m_xOpenTableHeap.Reset();
	m_xMapNodes.resize(size_t(_nRows * _nCols), NULL);
	memset(&m_xMapNodes[0], 0, sizeof(void*) * (_nRows * _nCols));

	//	first, add start node to open table and close table
	AStarNode* pStartNode = AddOpenTableElement(_nStartX, _nStartY, GetValueH(_nStartX, _nStartY, _nDestX, _nDestY), 0);
	pStartNode->bInCloseTable = true;

	//	now , do the search process
	AStarNode* pSelectNode = NULL;

	while (1) {
		pSelectNode = NULL;

		//	search for the min ValueF node
		if (!m_xOpenTableHeap.PopFront(astarnode_compare_lessorequal, &pSelectNode))
		{
			return false;
		}

		if (NULL == pSelectNode) {
			//	can't find the path
			return false;
		}

		//	add to close table
		pSelectNode->bInCloseTable = true;
		pSelectNode->bInOpenTable = false;

		//	search for the around to initialize open list
		bool bInsert = false;
		for (int i = 0; i < 8; i++) {
			int nNextX = pSelectNode->nPosX + s_nPosOffsetDiagonal[i * 2];
			int nNextY = pSelectNode->nPosY + s_nPosOffsetDiagonal[i * 2 + 1];

			int nFlag = _fnProvideNodeBasicInfo(nNextX, nNextY);

			if (AStarFlag_Ok != nFlag) {
				//	nothing
				continue;
			}

			//	is destination ?
			if (nNextX == _nDestX &&
				nNextY == _nDestY) {
					//	get the path !
					_fnAcceptPathResult(nNextX, nNextY);

					while (NULL != pSelectNode) {
						_fnAcceptPathResult(pSelectNode->nPosX, pSelectNode->nPosY);
						pSelectNode = pSelectNode->pParent;
					}

					return true;
			}

			//	in close table ?
			if(NULL != m_xMapNodes[nNextX + m_nMapCol * nNextY] &&
				m_xMapNodes[nNextX + m_nMapCol * nNextY]->bInCloseTable) {
					continue;
			}

			//	in open table ?
			if(NULL != m_xMapNodes[nNextX + m_nMapCol * nNextY]) {
				//	check G value again
				AStarNode* pOpenedNode = m_xMapNodes[nNextX + m_nMapCol * nNextY];

				if (pSelectNode->nValueG + s_nValueG[i] < pOpenedNode->nValueG) {
					pOpenedNode->nValueG = pSelectNode->nValueG + s_nValueG[i];
					pOpenedNode->pParent = pSelectNode;

					//	rebuild
					m_xOpenTableHeap.RebuildSlow(astarnode_compare_lessorequal);
				}

				continue;
			}

			AStarNode* pNode = AddOpenTableElement(nNextX, nNextY, GetValueH(nNextX, nNextY, _nDestX, _nDestY),
				pSelectNode->nValueG + GetValueG(pSelectNode->nPosX, pSelectNode->nPosY, nNextX, nNextY));
			pNode->pParent = pSelectNode;
		}
	}
}

bool
	AStarPathFinder::FindPath(int _nRows,
	int _nCols,
	int _nStartX,
	int _nStartY,
	int _nDestX,
	int _nDestY,
	AStarProvideNodeBasicInfo _fnProvideNodeBasicInfo,
	AStarAcceptPathResult _fnAcceptPathResult)
{
	bool bRet = _FindPath(_nRows, _nCols, _nStartX, _nStartY, _nDestX, _nDestY, _fnProvideNodeBasicInfo, _fnAcceptPathResult);

	//	clean up
	AStarNodeList::iterator it = m_xAllocateNodes.begin();

	for (it; it != m_xAllocateNodes.end(); ++it)
	{
		delete *it;
	}
	m_xAllocateNodes.clear();

	return bRet;
}