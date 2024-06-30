#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#define PARENT(i) (i-1)/2
#define LEFT(i) (2*(i)+1)
#define RIGHT(i) (2*(i)+2)
#define MAGIC_NUMBER 387261043
#define IS_LEAF(i, size) LEFT(i) >= size /*do it with the Heap itself!*/
#define LAST _heap->m_heapSize - 1

struct Heap
{
	Vector * m_vec;
	size_t m_heapSize;
	size_t m_magicNumber;
};

static void Heapify(Heap * _heap, size_t _index);
static void BubbleUp(Vector * _vector, size_t _index);
static ADTErr CheckHeapMaxParameters(Heap * _heap, int * _data);
static size_t FindBiggest(Heap * _heap, size_t _index);
static void Swap(Heap * _heap, size_t _index1, size_t _index2);
static ADTErr CheckHSParameters(Heap * _heap);

Heap* HeapBuild(Vector* _vec)
{
	Heap * temp;
	size_t size;
	register size_t i;
	if (_vec == NULL)
	{
		return NULL;
	}
	if ((temp = (Heap*)malloc(sizeof(Heap))) == NULL)
	{
		return NULL;
	}
	VectorItemsNum(_vec, &size);
	temp->m_vec = _vec;
	temp->m_heapSize = size;
	temp->m_magicNumber = MAGIC_NUMBER;
	if(temp->m_heapSize > 1)
	{
		for(i = PARENT((temp->m_heapSize) - 1); i >= 1; --i)
		{ 
			Heapify(temp, i);
		}
		Heapify(temp, 0);		
	}
	return temp;
}

void HeapDestroy(Heap* _heap)
{
	if (_heap == NULL || _heap->m_magicNumber != MAGIC_NUMBER)
	{
		return;
	}
	_heap->m_magicNumber = 0;
	free(_heap);
}

ADTErr HeapInsert(Heap* _heap, int _data)
{
	ADTErr check;
	size_t index;
	if (_heap == NULL)
	{
		return ERR_NOT_INITIALIZED;
	}
	if ((check = VectorAdd(_heap->m_vec, _data)) != ERR_OK)
	{
		return check;
	}
	VectorItemsNum(_heap->m_vec, &index);
	BubbleUp(_heap->m_vec, index - 1);
	++_heap->m_heapSize;
	return check;
}

ADTErr HeapMax(Heap* _heap, int* _data)
{
	ADTErr check;
	if((check = (CheckHeapMaxParameters(_heap, _data))) != ERR_OK)
	{
		return check;
	}
	check = VectorGet(_heap->m_vec, 0, _data);
	return check;
}

ADTErr HeapExtractMax(Heap* _heap, int* _data)
{
	size_t index;
	int last;
	ADTErr check;
	if ((check = CheckHeapMaxParameters(_heap, _data)) != ERR_OK)
	{
		return check;
	}
	
	VectorGet(_heap->m_vec, 0, _data);
	VectorDelete(_heap->m_vec, &last);
	VectorSet(_heap->m_vec, 0, last);
	VectorItemsNum(_heap->m_vec, &index); /*needs to check if there are items yet! missing test*/
	--_heap->m_heapSize;
	if(_heap->m_heapSize > 1)
	{
		Heapify(_heap, 0);		
	}
	return check;
	
}



size_t HeapItemsNum(Heap* _heap)
{
	if (_heap == NULL)
	{
		return 0;
	}
	return _heap->m_heapSize;
}

ADTErr HeapSort(Heap * _heap)
{
	size_t size;
	register size_t i;
	ADTErr check;
	if((check = CheckHSParameters(_heap)) != ERR_OK)
	{
		return check;
	}
	size = _heap->m_heapSize;
	for(i = 0; i < size; ++i)
	{
		Swap(_heap, 0, LAST);
		--_heap->m_heapSize;
		Heapify(_heap, 0);
	}
	return check;
}

void HeapPrint(Heap* _heap)
{
	if (_heap == NULL)
	{
		return;
	}
	VectorPrint(_heap->m_vec);
}


/*--------------------------------------------------------------------------------------*/
/*
static void Heapify(Vector * _vec, int _index) 
{
	size_t size;
	int  parent, lSon, rSon;
	VectorItemsNum(_vec, &size);
	if (LEFT(_index) >= size)
	{
		return;
	}
	Heapify(_vec, _index + 1);
	VectorGet(_vec, _index, &parent);
	VectorGet(_vec, LEFT(_index), &lSon);
	if(VectorGet(_vec, RIGHT(_index), &rSon) == ERR_OK)
	{
		if (parent >= lSon && parent >= rSon)
		{
			return;
		}
		if (parent < lSon && lSon > rSon)
		{
			VectorSet(_vec, _index, lSon);
			VectorSet(_vec, LEFT(_index), parent);
			return;
		}
		VectorSet(_vec, _index, rSon);
		VectorSet(_vec, RIGHT(_index), parent);
		return;
	}
	if (parent < lSon)
	{
		VectorSet(_vec, _index, lSon);
		VectorSet(_vec, LEFT(_index), parent);
		return;
	}
}
*/
static void Heapify(Heap * _heap, size_t _index)
{
	size_t size = _heap->m_heapSize, biggestIndex;
	if(IS_LEAF(_index, size))
	{
		return;
	}
	biggestIndex = FindBiggest(_heap, _index);
	if(biggestIndex != _index)
	{
		Swap(_heap, _index, biggestIndex);
		Heapify(_heap, biggestIndex);
	}
}

static size_t FindBiggest(Heap * _heap, size_t _index)
{
	size_t size = _heap->m_heapSize;
	int lSon, rSon, parent;
	if(RIGHT(_index) >= size)
	{
		VectorGet(_heap->m_vec, _index, &parent);
		VectorGet(_heap->m_vec, LEFT(_index), &lSon);
		if(lSon > parent)
		{
			return LEFT(_index);
		}
		return _index;
	}
	VectorGet(_heap->m_vec, _index, &parent);
	VectorGet(_heap->m_vec, LEFT(_index), &lSon);
	VectorGet(_heap->m_vec, RIGHT(_index), &rSon);
	if (lSon > parent && lSon > rSon)
	{
		return LEFT(_index);
	}
	if(rSon > parent)
	{
		return RIGHT(_index);
	}
	return _index;
}

static void Swap(Heap * _heap, size_t _index1, size_t _index2)
{
	int item1, item2;
	VectorGet(_heap->m_vec, _index1, &item1);
	VectorGet(_heap->m_vec, _index2, &item2);
	VectorSet(_heap->m_vec, _index1, item2);
	VectorSet(_heap->m_vec, _index2, item1);	
}

static void BubbleUp(Vector * _vector, size_t _index)
{
	int son, parent;
	size_t tempIndex = _index;
	VectorGet(_vector, _index, &son);
	VectorGet(_vector, PARENT(_index), &parent);
	while (son > parent || tempIndex != 0)
	{
		VectorSet(_vector, PARENT(tempIndex), son); /*do in a different function!*/
		VectorSet(_vector, tempIndex, parent);
		tempIndex = PARENT(tempIndex);
		VectorGet(_vector, PARENT(_index), &parent);
	}
	return;
}

static ADTErr CheckHeapMaxParameters(Heap * _heap, int * _data)
{
	if (_heap == NULL || _data == NULL)
	{
		return ERR_POINTER_NULL;
	}
	if (_heap->m_heapSize == 0)
	{
		return ERR_NO_ITEMS;
	}
	return ERR_OK;
}


static ADTErr CheckHSParameters(Heap * _heap)
{
	if(_heap == NULL)
	{
		return ERR_POINTER_NULL;
	}
	if(_heap->m_magicNumber != MAGIC_NUMBER)
	{
		return ERR_NOT_INITIALIZED;
	}
	return ERR_OK;
}
