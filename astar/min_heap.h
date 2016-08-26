#ifndef _INC_MIN_HEAP_
#define _INC_MIN_HEAP_
//////////////////////////////////////////////////////////////////////////
#include <vector>
//////////////////////////////////////////////////////////////////////////
struct AstarNode;

typedef std::vector<AstarNode*> NodeVector;

//////////////////////////////////////////////////////////////////////////
class NodeMinHeap
{
public:
	NodeMinHeap();
	~NodeMinHeap();

public:
	void Reset();
	void Resize(size_t _uSize);

	int Insert(AstarNode* _pNode);
	AstarNode* PopFront();
	int Rebuild(AstarNode* _pNode);
	int RebuildSlow();

	inline int GetItemCount()
	{
		return m_nNodeCount;
	}

	void Dump();

private:
	AstarNode* getNodeByIndex(int _nNodeIndex);
	AstarNode* findParentNode(int _nNodeIndex);
	int findLeftChildIndex(int _nNodeIndex);
	int findRightChildIndex(int _nNodeIndex);

private:
	NodeVector m_xNodes;
	int m_nNodeCount;
};
//////////////////////////////////////////////////////////////////////////
#endif