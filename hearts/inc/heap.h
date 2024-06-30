#ifndef __HEAP_H__
#define __HEAP_H__
#include "ADTDefs.h"
#include "vector.h"

typedef struct Heap Heap;

/*description - Creates a Heap data structure from a vector
input - a Vector pointer
output - a Heap Pointer or NULL
Error - NULL if the vector pointer is NULL or if the Heap allocation fails*/
Heap* HeapBuild(Vector* _vec); /* O(n) */

/*description - destroys a Heap data structure
input - a Heap pointer
output - none
Error - none */
void HeapDestroy(Heap* _heap);

/*description - inserts an integer to a Heap
input - a Heap pointer and an integer
output - ERR_OK if successful and ERR_NOT_INITIALIZED if not
Error - ERR_NOT_INITIALIZED if the Heap Pointer is NULL or if int insertion fails*/
ADTErr HeapInsert(Heap* _heap, int _data); /* O(log n) */

/*description - Gets the max item in a heap
input - a Heap pointer and an integer variable address
output - ERR_OK if successful, ERR_POINTER_NULL, ERR_NO_ITEMS, ERR_UNDERFLOW
Error - ERR_POINTER_NULL - if either of the pointers are NULL
		ERR_NO_ITEMS if there are no items in the Heap
		ERR_UNDERFLOW if there are no items in the Vector */
ADTErr HeapMax(Heap* _heap, int* _data);

/*description - removes the max item in a heap
input - a Heap pointer and an integer variable address
output - ERR_OK if successful, ERR_POINTER_NULL, ERR_NO_ITEMS, ERR_UNDERFLOW
Error - ERR_POINTER_NULL - if either of the pointers are NULL
		ERR_NO_ITEMS if there are no items in the Heap
		ERR_UNDERFLOW if there are no items in the Vector */
ADTErr HeapExtractMax(Heap* _heap, int* _data); /* O(log n) */

/*description - returns the number of items in a Heap
input - a Heap pointer
output - the number of items in the Heap or 0
Error - if the Heap pointer is NULL returns 0 */
size_t HeapItemsNum(Heap* _heap);


ADTErr HeapSort(Heap* _heap);

/*description - prints the items in the heap
input - Heap pointer
output - a print of the contents of the Heap
Error - none*/
void HeapPrint(Heap* _heap);


#endif /*__HEAP_H__*/
