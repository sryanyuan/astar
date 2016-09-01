#ifndef _INC_MINHEAP_
#define _INC_MINHEAP_
//////////////////////////////////////////////////////////////////////////
#include <vector>
/*
	min heap is binary tree that you can always get the min element in root node
*/
//////////////////////////////////////////////////////////////////////////
template<class T>
class MinHeap
{
public:
	typedef std::vector<T> ElementContainer;
	//	determine which element is less or equal , when left param is less than right param , you should return true
	typedef bool (*SortLessFunc)(const T&, const T&);
	//	foreach
	typedef void (*ForeachFunc)(int, const T&);

public:
	MinHeap()
	{
		m_nNodeCount = 0;
	}
	~MinHeap()
	{

	}

public:
	//	set the max size of the heap
	void Resize(size_t _uSize, const T& _default)
	{
		m_xNodes.resize(_uSize, _default);
	}

	//	reset all elements in container
	void Reset()
	{
		size_t uSize = m_xNodes.size();

		if (0 != uSize)
		{
			memset(&m_xNodes[0], 0, sizeof(T) * uSize);
			m_nNodeCount = 0;
		}
	}

	//	get the element count
	int GetItemCount()
	{
		return m_nNodeCount;
	}

	//	insert a element to the heap , return negative value if insert failed , or return the insert node index
	int Insert(const T& _pNode, SortLessFunc _fnSort)
	{
		//	first put it in the tail
		if (size_t(m_nNodeCount) >= m_xNodes.size()) {
			return -1;
		}

		const T& pCurNode = _pNode;
		int nCurIndex = m_nNodeCount;
		m_xNodes[m_nNodeCount++] = _pNode;

		if (m_nNodeCount == 1) {
			return nCurIndex;
		}

		//	find the parent node
		int nParentIndex = findParentNodeIndex(nCurIndex);
		const T* pParent = getNodeByIndex(nParentIndex);
		bool bInserted = false;

		while(NULL != pParent)
		{
			if (_fnSort(pCurNode, *pParent)) {
				//	swap
				m_xNodes[nParentIndex] = pCurNode;
				m_xNodes[nCurIndex] = *pParent;
				nCurIndex = nParentIndex;

				//	get parent
				nParentIndex = findParentNodeIndex(nCurIndex);
				pParent = getNodeByIndex(nParentIndex);
			} else {
				//	ok
				bInserted = true;
				break;
			}
		}

		if (bInserted) {
			return nCurIndex;
		} else {
			return -1;
		}
	}

	//	pop the root item and rebuild the tree , return NULL if the tree is empty
	bool PopFront(SortLessFunc _fnSort, T* _pResult)
	{
		//	erase the root node
		if (m_nNodeCount == 0)
		{
			return false;
		}

		T xPopedNode = *getNodeByIndex(0);

		if (m_nNodeCount == 1)
		{
			--m_nNodeCount;

			*_pResult = xPopedNode;
			return true;
		}

		if (m_nNodeCount == 2)
		{
			m_xNodes[0] = m_xNodes[1];
			--m_nNodeCount;

			*_pResult = xPopedNode;
			return true;
		}

		int nEmptyIndex = 0;
		int nLeftChildIndex = findLeftChildIndex(nEmptyIndex);
		int nRightChildIndex = findRightChildIndex(nEmptyIndex);
		T* pLeftChild = getNodeByIndex(nLeftChildIndex);
		T* pRightChild = getNodeByIndex(nRightChildIndex);
		T* pTailNode = getNodeByIndex(m_nNodeCount - 1);

		if (m_nNodeCount == 3)
		{
			if (_fnSort(*pLeftChild, *pRightChild))
			{
				m_xNodes[0] = *pLeftChild;
				m_xNodes[1] = *pRightChild;
				--m_nNodeCount;

				*_pResult = xPopedNode;
				return true;
			}
			else
			{
				m_xNodes[0] = *pRightChild;
				--m_nNodeCount;

				*_pResult = xPopedNode;
				return true;
			}
		}

		while(NULL != pLeftChild ||
			NULL != pRightChild)
		{
			int nParentIndex = findParentNodeIndex(nEmptyIndex);
			T* pEmptyParent = getNodeByIndex(nParentIndex);

			//	tail node match the rule
			bool bMatch = true;

			if (NULL != pEmptyParent)
			{
				if (!_fnSort(*pEmptyParent, *pTailNode))
				{
					bMatch = false;
				}
			}

			if (NULL != pLeftChild)
			{
				if (_fnSort(*pLeftChild, *pTailNode))
				{
					bMatch = false;
				}
			}

			if (NULL != pRightChild)
			{
				if (_fnSort(*pRightChild, *pTailNode))
				{
					bMatch = false;
				}
			}

			if (bMatch)
			{
				//	find the position
				m_xNodes[nEmptyIndex] = *pTailNode;
				--m_nNodeCount;
				
				*_pResult = xPopedNode;
				return true;
			}
			else
			{
				//	down the min branch
				if (NULL != pLeftChild &&
					NULL != pRightChild)
				{
					if (_fnSort(*pLeftChild, *pRightChild))
					{
						int nPrevIndex = nLeftChildIndex;
						m_xNodes[nEmptyIndex] = *pLeftChild;
						nEmptyIndex = nPrevIndex;
					}
					else
					{
						int nPrevIndex = nRightChildIndex;
						m_xNodes[nEmptyIndex] = *pRightChild;
						nEmptyIndex = nPrevIndex;
					}
				}
				else if (NULL != pLeftChild)
				{
					int nPrevIndex = nLeftChildIndex;
					m_xNodes[nEmptyIndex] = *pLeftChild;
					nEmptyIndex = nPrevIndex;
				}
				else if (NULL != pRightChild)
				{
					int nPrevIndex = nRightChildIndex;
					m_xNodes[nEmptyIndex] = *pRightChild;
					nEmptyIndex = nPrevIndex;
				}

				nLeftChildIndex = findLeftChildIndex(nEmptyIndex);
				nRightChildIndex = findRightChildIndex(nEmptyIndex);
				pLeftChild = getNodeByIndex(nLeftChildIndex);
				pRightChild = getNodeByIndex(nRightChildIndex);
			}
		}

		if (nEmptyIndex != m_nNodeCount - 1)
		{
			m_xNodes[nEmptyIndex] = *pTailNode;
		}
		--m_nNodeCount;

		*_pResult = xPopedNode;
		return true;
	}

	//	rebuild the tree , only when the tree item value is changed
	int RebuildSlow(SortLessFunc _fnSort)
	{
		//	NOTE : just append all ...
		if(m_nNodeCount <= 1)
		{
			return 0;
		}

		ElementContainer xVec = m_xNodes;
		size_t nPrevNodeCount = size_t(m_nNodeCount);
		Reset();

		for (size_t i = 0; i < nPrevNodeCount; ++i)
		{
			Insert(xVec[i], _fnSort);
		}

		return 0;
	}

	//	dump all item
	MinHeap<T>& Foreach(ForeachFunc _fnForeach)
	{
		for (int i = 0; i < m_nNodeCount; ++i)
		{
			_fnForeach(i, m_xNodes[i]);
		}

		return *this;
	}

	//	debug validate
	bool Validate(SortLessFunc _fnSort)
	{
		if (m_nNodeCount <= 1)
		{
			return true;
		}

		//	get root
		int nParentIndex = 0;
		T* pParent = getNodeByIndex(nParentIndex);

		return validateNode(_fnSort, nParentIndex, pParent);
	}

private:
	bool validateNode(SortLessFunc _fnSort, int _nNodeIndex, T* _pNode)
	{
		if (NULL != _pNode)
		{
			int nLeftChildIndex = findLeftChildIndex(_nNodeIndex);
			int nRightChildIndex = findLeftChildIndex(_nNodeIndex);
			T* pLeftChild = getNodeByIndex(nLeftChildIndex);
			T* pRightChild = getNodeByIndex(nRightChildIndex);

			if (NULL != pLeftChild)
			{
				if (!_fnSort(*_pNode, *pLeftChild))
				{
					return false;
				}
				
				if (!validateNode(_fnSort, nLeftChildIndex, pLeftChild))
				{
					return false;
				}
			}

			if (NULL != pRightChild)
			{
				if (!_fnSort(*_pNode, *pRightChild))
				{
					return false;
				}

				if (!validateNode(_fnSort, nRightChildIndex, pRightChild))
				{
					return false;
				}
			}
		}

		return true;
	}
	T* getNodeByIndex(int _nNodeIndex)
	{
		if (_nNodeIndex < 0 ||
			_nNodeIndex >= m_nNodeCount)
		{
			return NULL;
		}
		return &m_xNodes[_nNodeIndex];
	}
	int findParentNodeIndex(int _nNodeIndex)
	{
		if (_nNodeIndex == 0) {
			return -1;
		}

		int nParentIndex = (_nNodeIndex - 1) / 2;

		if (nParentIndex >= m_nNodeCount) {
			return -1;
		}

		return nParentIndex;
	}
	int findLeftChildIndex(int _nNodeIndex)
	{
		return _nNodeIndex * 2 + 1;
	}
	int findRightChildIndex(int _nNodeIndex)
	{
		return _nNodeIndex * 2 + 2;
	}

public:
	ElementContainer m_xNodes;
	int m_nNodeCount;
};
//////////////////////////////////////////////////////////////////////////
#endif