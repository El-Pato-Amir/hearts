#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "ADTDefs.h"
#define BLOCKSIZE_FACTOR 2
#define MAGIC_NUMBER 592840643

struct Vector
{
	int* m_items;
	size_t m_originalSize;
	size_t m_size;
	size_t m_nItems;
	size_t m_blockSize;
	size_t m_magicNumber;	
};

ADTErr CheckAddParameters(Vector * _vector);
ADTErr CheckCreateParameters(size_t _initialSize, size_t _extensionBlockSize);
ADTErr CheckDestroyParameters(Vector * _vector);
ADTErr CheckDeleteParameters(Vector * _vector, int * _item);
ADTErr CheckDecreaseParameters(Vector * _vector);
ADTErr CheckGetParameters(Vector *_vector, size_t _index, int *_item);
ADTErr CheckSetParameters (Vector *_vector, size_t _index);
ADTErr CheckItemsNumParameters(Vector *_vector, size_t * _numOfItems);
ADTErr CheckPrintParameters(Vector * _vector);


Vector* VectorCreate(size_t _initialSize, size_t _extensionBlockSize)
{
	Vector * address;
	if (CheckCreateParameters( _initialSize, _extensionBlockSize) != ERR_OK)
	{
		return NULL;
	}
	if ((address = (Vector*)malloc(sizeof(Vector))) == NULL)
	{
		return NULL;
	}
	if (((address->m_items) = (int*)calloc(_initialSize, sizeof(Vector*))) == NULL )
	{
		free(address);
		return NULL;
	} 
	address -> m_originalSize = _initialSize;
	address -> m_size = _initialSize;
	address -> m_nItems = 0;
	address -> m_blockSize = _extensionBlockSize;
	address -> m_magicNumber = MAGIC_NUMBER;
	return address;	
}

void VectorDestroy(Vector* _vector)
{
	if (CheckDestroyParameters(_vector) != ERR_OK)
	{
		return;
	}
	_vector->m_magicNumber = 0;
	free(_vector->m_items);
	free(_vector);		
}

ADTErr VectorAdd(Vector *_vector, int _item)
{
	ADTErr check;
	if ((check = CheckAddParameters(_vector)) != ERR_OK)
	{
		return check;
	}
	_vector->m_items[_vector->m_nItems] = _item;
	++(_vector->m_nItems);
	return ERR_OK;
}
	
ADTErr VectorDelete(Vector *_vector, int* _item)
{
	ADTErr check;
	if ((check = CheckDeleteParameters(_vector, _item)) != ERR_OK)
	{
		return check;
	}
	*_item = _vector->m_items[(_vector->m_nItems) - 1];
	--_vector->m_nItems;
	if(CheckDecreaseParameters(_vector) == ERR_OK)
	{
		_vector->m_size -= _vector->m_blockSize;
	}
	return check;	
}

ADTErr VectorGet(Vector *_vector, size_t _index, int *_item)
{
	ADTErr check;
	if ((check = CheckGetParameters(_vector, _index, _item)) != ERR_OK)
	{
		return check;
	}
	*_item = _vector->m_items[_index];
	return check;
}

ADTErr VectorSet(Vector *_vector, size_t _index, int _item)
{
	ADTErr check;
	if ((check = CheckSetParameters(_vector, _index)) != ERR_OK)
	{
		return check;
	}
	_vector->m_items[_index] = _item;
	return check;
}

ADTErr VectorItemsNum(Vector *_vector, size_t* _numOfItems)
{
	ADTErr check;
	if ((check = CheckItemsNumParameters(_vector, _numOfItems)) != ERR_OK)
	{
		return check;
	}
	*_numOfItems = _vector->m_nItems;
	return check;
}

void VectorPrint(Vector *_vector)
{
	int i;
	ADTErr check;
	if ((check = CheckPrintParameters(_vector)) != ERR_OK)
	{
		return;
	}
/*	printf("\nVector size: %lu\nOriginal size: %lu\nNumber of items in Vector: %lu\nBlock size: %lu\n", _vector->m_size, _vector->m_originalSize, _vector->m_nItems, _vector->m_blockSize);*/
	printf("\nList of items:\n");
	for (i = 0; i < _vector->m_nItems; ++i)
	{
		printf("%d\n", _vector->m_items[i]);	
	}
}

/*---------------------------------------------------------------------------------------------------------------*/
ADTErr CheckCreateParameters(size_t _initialSize, size_t _extensionBlockSize)
{
	if( _initialSize == 0 && _extensionBlockSize == 0)
	{
		return ERR_NOT_INITIALIZED;
	}
	return ERR_OK;
}

ADTErr CheckDestroyParameters(Vector * _vector)
{
	if (_vector == NULL)
	{
		return ERR_NOT_INITIALIZED;
	}
	if (_vector->m_magicNumber != MAGIC_NUMBER)
	{
		return ERR_NOT_INITIALIZED;
	}
	return ERR_OK;
}

ADTErr CheckAddParameters(Vector * _vector)
{
	int* temp;
	if (_vector == NULL)
	{
		return ERR_POINTER_NULL;
	}
	if (_vector->m_magicNumber != MAGIC_NUMBER)
	{
		return ERR_NOT_INITIALIZED;
	}
	if ( _vector->m_nItems == _vector->m_size)
	{
		if (_vector->m_blockSize == 0)
		{
			return ERR_OVERFLOW;
		}
		if(( temp = (int*)realloc((_vector->m_items), (((_vector->m_size) + (_vector->m_blockSize)) * sizeof(int)))) == NULL)
		{
			return ERR_REALLOCATION_FAILED;
		}
		_vector->m_items = temp;
		_vector->m_size += _vector->m_blockSize;
	}
	return ERR_OK;
	
}


ADTErr CheckDeleteParameters(Vector * _vector, int * _item)
{
	if (_vector == NULL || _item == NULL)
	{
		return ERR_NOT_INITIALIZED;
	}
	if (_vector->m_magicNumber != MAGIC_NUMBER)
	{
		return ERR_NOT_INITIALIZED;
	}
	if (_vector->m_nItems == 0)
	{
		return ERR_UNDERFLOW;
	}
	return ERR_OK;
}

ADTErr CheckDecreaseParameters(Vector * _vector)
{
	int* temp;
	if(_vector->m_blockSize != 0 && 
	_vector->m_size > _vector->m_originalSize &&
	 ((_vector->m_size) - (_vector->m_nItems)) == (BLOCKSIZE_FACTOR*(_vector->m_blockSize)))
	{
		if((temp = (int*)realloc((_vector->m_items), (((_vector->m_size) - (_vector->m_blockSize)) * sizeof(int)))) == NULL)
		{
			return ERR_REALLOCATION_FAILED;
		}
		_vector->m_items = temp;
		return ERR_OK;
	}
	return ERR_GENERAL;
}

ADTErr CheckGetParameters(Vector *_vector, size_t _index, int *_item)
{
	if (_vector == NULL || _item == NULL)
	{
		return ERR_POINTER_NULL;
	}
	if (_vector->m_magicNumber != MAGIC_NUMBER)
	{
		return ERR_NOT_INITIALIZED;
	}
	if (_vector->m_nItems == 0)
	{
		return ERR_UNDERFLOW;
	}
	if(_index >= _vector->m_nItems)
	{
		return ERR_WRONG_INDEX;
	}
	return ERR_OK;
}

ADTErr CheckSetParameters (Vector *_vector, size_t _index)
{
	if (_vector == NULL)
	{
		return ERR_POINTER_NULL;
	}
	if (_vector->m_magicNumber != MAGIC_NUMBER)
	{
		return ERR_NOT_INITIALIZED;
	}
	if(_vector->m_nItems == 0)
	{
		return ERR_NO_ITEMS;
	}
	if(_index >= _vector->m_nItems)
	{
		return ERR_WRONG_INDEX;
	}
	return ERR_OK;	
}

ADTErr CheckItemsNumParameters(Vector *_vector, size_t * _numOfItems)
{
	if (_vector == NULL || _numOfItems == NULL)
	{
		return ERR_POINTER_NULL;
	}
	if (_vector->m_magicNumber != MAGIC_NUMBER)
	{
		return ERR_NOT_INITIALIZED;
	}
	return ERR_OK;
}

ADTErr CheckPrintParameters(Vector * _vector)
{
	if (_vector == NULL || _vector->m_items == NULL)
	{
		return ERR_POINTER_NULL;
	}
	if (_vector->m_magicNumber != MAGIC_NUMBER)
	{
		return ERR_NOT_INITIALIZED;
	}
	return ERR_OK;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------*/

int * GetITEMS(Vector * _vector)
{
	if (_vector != NULL)
	{
		return _vector->m_items;
	}
	return NULL;	
}

size_t GetOriginalSize(Vector * _vector)
{
	if (_vector != NULL)
	{
		return _vector->m_originalSize;
	}
	return -1;
}

size_t GetSize(Vector * _vector)
{
	if (_vector != NULL)
	{
		return _vector->m_size;
	}
	return -1;
}

size_t GetNITEMS(Vector * _vector)
{
	if (_vector != NULL)
	{
		return _vector->m_nItems;
	}
	return -1;
}

size_t GetBlockSize(Vector * _vector)
{
	if (_vector != NULL)
	{
		return _vector->m_blockSize;
	}
	return -1;
}

size_t GetMagicNumber(Vector * _vector)
{
	if (_vector != NULL)
	{
		return _vector->m_magicNumber;
	}
	return -1;
}
