#include "min_heap.h"
#include "astar.h"
//////////////////////////////////////////////////////////////////////////
NodeMinHeap::NodeMinHeap()
{
	Reset();
}

NodeMinHeap::~NodeMinHeap()
{

}

void NodeMinHeap::Reset()
{
	size_t uSize = m_xNodes.size();
	if (0 != uSize)
	{
		memset(&m_xNodes[0], 0, sizeof(AstarNode*) * uSize);
	}
	
	m_nNodeCount = 0;
}

void NodeMinHeap::Resize(size_t _uSize)
{
	m_xNodes.resize(_uSize, NULL);
}

int NodeMinHeap::Insert(AstarNode* _pNode)
{
	//	first put it in the tail
	if (size_t(m_nNodeCount) >= m_xNodes.size()) {
		return -1;
	}

	AstarNode* pCurNode = _pNode;
	int nCurIndex = m_nNodeCount;
	m_xNodes[m_nNodeCount++] = _pNode;
	_pNode->nHeapIndex = nCurIndex;

	if (m_nNodeCount == 1) {
		return nCurIndex;
	}

	//	find the parent node
	AstarNode* pParent = findParentNode(nCurIndex);
	bool bInserted = false;

	while(NULL != pParent)
	{
		if (pCurNode->nValueG + pCurNode->nValueH < pParent->nValueG +  pParent->nValueH) {
			//	swap
			m_xNodes[pParent->nHeapIndex] = pCurNode;
			m_xNodes[nCurIndex] = pParent;
			pCurNode->nHeapIndex = pParent->nHeapIndex;
			pParent->nHeapIndex = nCurIndex;
			nCurIndex = pCurNode->nHeapIndex;

			//	get parent
			pParent = findParentNode(pCurNode->nHeapIndex);
		} else {
			//	ok
			bInserted = true;
			break;
		}
	}

	if (bInserted) {
		return pCurNode->nHeapIndex;
	} else {
		return -1;
	}
}

AstarNode* NodeMinHeap::PopFront()
{
	AstarNode* pPopedNode = NULL;

	//	erase the root node
	if (m_nNodeCount == 0)
	{
		return NULL;
	}

	pPopedNode = m_xNodes[0];

	if (m_nNodeCount == 1)
	{
		m_xNodes[0] = NULL;
		--m_nNodeCount;
		return pPopedNode;
	}

	if (m_nNodeCount == 2)
	{
		m_xNodes[0] = m_xNodes[1];
		m_xNodes[1] = NULL;
		--m_nNodeCount;

		//	update heap index
		m_xNodes[0]->nHeapIndex = 0;

		return pPopedNode;
	}

	int nEmptyIndex = 0;
	AstarNode* pLeftChild = getNodeByIndex(findLeftChildIndex(nEmptyIndex));
	AstarNode* pRightChild = getNodeByIndex(findRightChildIndex(nEmptyIndex));
	AstarNode* pTailNode = getNodeByIndex(m_nNodeCount - 1);

	if (m_nNodeCount == 3)
	{
		if (pLeftChild->GetValueF() < pRightChild->GetValueF())
		{
			m_xNodes[0] = pLeftChild;
			pLeftChild->nHeapIndex = 0;
			m_xNodes[1] = pRightChild;
			pRightChild->nHeapIndex = 1;
			--m_nNodeCount;

			return pPopedNode;
		}
		else
		{
			m_xNodes[0] = pRightChild;
			pRightChild->nHeapIndex = 0;
			--m_nNodeCount;

			return pPopedNode;
		}
	}

	while(NULL != pLeftChild ||
		NULL != pRightChild)
	{
		AstarNode* pEmptyParent = findParentNode(nEmptyIndex);

		//	tail node match the rule
		bool bMatch = true;

		if (NULL != pEmptyParent)
		{
			if (pEmptyParent->GetValueF() > pTailNode->GetValueF())
			{
				bMatch = false;
			}
		}

		if (NULL != pLeftChild)
		{
			if (pLeftChild->GetValueF() < pTailNode->GetValueF())
			{
				bMatch = false;
			}
		}

		if (NULL != pRightChild)
		{
			if (pRightChild->GetValueF() < pTailNode->GetValueF())
			{
				bMatch = false;
			}
		}

		if (bMatch)
		{
			//	find the position
			m_xNodes[nEmptyIndex] = pTailNode;
			pTailNode->nHeapIndex = nEmptyIndex;
			m_xNodes[--m_nNodeCount] = NULL;
			return pPopedNode;
		}
		else
		{
			//	down the min branch
			if (NULL != pLeftChild &&
				NULL != pRightChild)
			{
				if (pLeftChild->GetValueF() < pRightChild->GetValueF())
				{
					int nPrevIndex = pLeftChild->nHeapIndex;
					m_xNodes[nEmptyIndex] = pLeftChild;
					pLeftChild->nHeapIndex = nEmptyIndex;
					nEmptyIndex = nPrevIndex;
					m_xNodes[nEmptyIndex] = NULL;
				}
				else
				{
					int nPrevIndex = pRightChild->nHeapIndex;
					m_xNodes[nEmptyIndex] = pRightChild;
					pRightChild->nHeapIndex = nEmptyIndex;
					nEmptyIndex = nPrevIndex;
					m_xNodes[nEmptyIndex] = NULL;
				}
			}
			else if (NULL != pLeftChild)
			{
				int nPrevIndex = pLeftChild->nHeapIndex;
				m_xNodes[nEmptyIndex] = pLeftChild;
				pLeftChild->nHeapIndex = nEmptyIndex;
				nEmptyIndex = nPrevIndex;
				m_xNodes[nEmptyIndex] = NULL;
			}
			else if (NULL != pRightChild)
			{
				int nPrevIndex = pRightChild->nHeapIndex;
				m_xNodes[nEmptyIndex] = pRightChild;
				pRightChild->nHeapIndex = nEmptyIndex;
				nEmptyIndex = nPrevIndex;
				m_xNodes[nEmptyIndex] = NULL;
			}

			pLeftChild = getNodeByIndex(findLeftChildIndex(nEmptyIndex));
			pRightChild = getNodeByIndex(findRightChildIndex(nEmptyIndex));
		}
	}

	if (nEmptyIndex != pTailNode->nHeapIndex)
	{
		m_xNodes[nEmptyIndex] = pTailNode;
		pTailNode->nHeapIndex = nEmptyIndex;
		--m_nNodeCount;
	}

	return pPopedNode;
}

AstarNode* NodeMinHeap::getNodeByIndex(int _nNodeIndex)
{
	if (_nNodeIndex < 0 ||
		_nNodeIndex >= m_nNodeCount)
	{
		return NULL;
	}

	return m_xNodes[_nNodeIndex];
}

AstarNode* NodeMinHeap::findParentNode(int _nNodeIndex)
{
	if (_nNodeIndex == 0) {
		return NULL;
	}

	int nParentIndex = (_nNodeIndex - 1) / 2;

	if (nParentIndex >= m_nNodeCount) {
		return NULL;
	}

	return m_xNodes[nParentIndex];
}

int NodeMinHeap::Rebuild(AstarNode* _pNode)
{
	//	TODO : rebuild faster

	return 0;
}

int NodeMinHeap::RebuildSlow()
{
	//	NOTE : just append all ...
	if(m_nNodeCount <= 1)
	{
		return 0;
	}

	NodeVector xVec(m_nNodeCount);
	memcpy(&xVec[0], &m_xNodes[0], sizeof(void*) * m_nNodeCount);

	Reset();

	for (size_t i = 0; i < xVec.size(); ++i)
	{
		Insert(xVec[i]);
	}

	return 0;
}

int NodeMinHeap::findLeftChildIndex(int _nNodeIndex)
{
	return _nNodeIndex * 2 + 1;
}

int NodeMinHeap::findRightChildIndex(int _nNodeIndex)
{
	return _nNodeIndex * 2 + 2;
}

void NodeMinHeap::Dump()
{
	for (int i = 0; i < m_nNodeCount; ++i)
	{
		printf("%d ", m_xNodes[i]->GetValueF());
	}
}