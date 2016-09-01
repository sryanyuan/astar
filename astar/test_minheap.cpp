#include "minheap.h"
#include <time.h>
//////////////////////////////////////////////////////////////////////////
bool MinHeapIntElementSortLess(const int& _l, const int& _r)
{
	return _l <= _r;
}

void MinHeapIntElementForeach(int, const int& _l)
{
	printf("%d ", _l);
}

void TestMinheap_01()
{
	MinHeap<int> heap;
	heap.Resize(100, 0);

	heap.Insert(1, MinHeapIntElementSortLess);
	heap.Insert(10, MinHeapIntElementSortLess);
	heap.Insert(9, MinHeapIntElementSortLess);
	heap.Insert(8, MinHeapIntElementSortLess);
	heap.Insert(7, MinHeapIntElementSortLess);
	heap.Insert(8, MinHeapIntElementSortLess);
	heap.Insert(12, MinHeapIntElementSortLess);
	heap.Insert(9, MinHeapIntElementSortLess);
	heap.Insert(19, MinHeapIntElementSortLess);

	if (!heap.Validate(MinHeapIntElementSortLess))
	{
		printf(__FUNCTION__" failed !!\n");
		return;
	}

	//	output
	do 
	{
		printf("items:\n");
		heap.Foreach(MinHeapIntElementForeach);
		printf("\npoped one:\n");
		int nPoped = 0;
		if (heap.PopFront(MinHeapIntElementSortLess, &nPoped))
		{
			printf("poped one element:%d\n", nPoped);
		}

		if (!heap.Validate(MinHeapIntElementSortLess))
		{
			printf(__FUNCTION__" failed !!\n");
			return;
		}
	} while (heap.GetItemCount() != 0);

	printf(__FUNCTION__" passed\n");
}

void TestMinheap_02()
{
	MinHeap<int> heap;
	int nHeapSize = 100;
	heap.Resize(nHeapSize, 0);

	srand((unsigned int)time(NULL));

	for (int i = 0; i < nHeapSize; ++i)
	{
		heap.Insert(rand() % (nHeapSize * 10), MinHeapIntElementSortLess);
	}

	if (!heap.Validate(MinHeapIntElementSortLess))
	{
		printf(__FUNCTION__" failed !!\n");
		return;
	}

	//	output
	do 
	{
		printf("items:\n");
		heap.Foreach(MinHeapIntElementForeach);
		printf("\npoped one:\n");
		int nPoped = 0;
		if (heap.PopFront(MinHeapIntElementSortLess, &nPoped))
		{
			printf("poped one element:%d\n", nPoped);
		}

		if (!heap.Validate(MinHeapIntElementSortLess))
		{
			printf(__FUNCTION__" failed !!\n");
			return;
		}
	} while (heap.GetItemCount() != 0);

	printf(__FUNCTION__" passed\n");
}


void TestMinheap()
{
	TestMinheap_01();
	TestMinheap_02();
}