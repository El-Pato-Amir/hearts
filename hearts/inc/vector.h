#ifndef __VECTOR_H__
#define __VECTOR_H__
#include "ADTDefs.h"

typedef struct Vector Vector;

/*description - Creates a Dynamic Vector
input - initial size and block size
output - Vector pointer
errors - NULL */
Vector* VectorCreate(size_t _initialSize, size_t _extensionBlockSize);

/*description - frees a Dynamic Vector
input - Vector pointer
output - none
errors - none */
void VectorDestroy(Vector* _vector);

/*description - Adds an item to the array saved in the Vector
input - Vector pointer and an integer to add
output - ERR_OK
errors - ERR_POINTER_NULL, ERR_OVERFLOW */
ADTErr VectorAdd(Vector *_vector, int _item);

/*description - Deletes the last item added to the Vector and saves it to an int variable
input - Vector pointer and an integer variable address
output - ERR_OK
errors - ERR_POINTER_NULL, ERR_UNDERFLOW, ERR_REALLOCATION_FAILED */
ADTErr VectorDelete(Vector *_vector, int* _item);

/*description - Gets an item from the array by the index and saves it to an int variable
input - Vector pointer, index, integer variable address
output - ERR_OK
errors - ERR_POINTER_NULL, ERR_WRONG_INDEX */
ADTErr VectorGet(Vector *_vector, size_t _index, int *_item);

/*description - Gets an integer and an index and sets said integer inside the array 
input - Vector pointer, index, int variable to set
output - ERR_OK
errors - ERR_POINTER_NULL, ERR_WRONG_INDEX */
ADTErr VectorSet(Vector *_vector, size_t _index, int _item);

/*description - Gets the number of items currently in the Vector an save it to a size_t variable
input - Vector pointer and a size_t variable address
output - ERR_OK
errors - ERR_POINTER_NULL, ERR_NO_ITEMS */
ADTErr VectorItemsNum(Vector *_vector, size_t* _numOfItems);

/*description - Prints all the data saved in the Vector
input - Vector pointer
output - printed list
errors - none */
void VectorPrint(Vector *_vector);


#endif /*__VECTOR_H__*/

