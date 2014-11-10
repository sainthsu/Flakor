/****************************************************************************
Copyright (c) 2014 flakor.org

http://www.flakor.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CArray.h"
#include "base/lang/Object.h"

FLAKOR_NS_BEGIN

/** Allocates and initializes a new array with specified capacity */
fkArray* fkArrayNew(unsigned int capacity) 
{
	if (capacity == 0)
		capacity = 1;
	
	fkArray *arr = (fkArray*)malloc( sizeof(fkArray) );
	arr->num = 0;
	arr->arr =  (Object**)calloc(capacity, sizeof(Object*));
	arr->max = capacity;
	
	return arr;
}

/** Frees array after removing all remaining objects. Silently ignores NULL arr. */
void fkArrayFree(fkArray*& arr)
{
    if( arr == NULL )
    {
        return;
    }
	fkArrayRemoveAllObjects(arr);
	
	free(arr->arr);
	free(arr);

    arr = NULL;
}

void fkArrayDoubleCapacity(fkArray *arr)
{
	arr->max *= 2;
	Object** newArr = (Object**)realloc( arr->arr, arr->max * sizeof(Object*) );
	// will fail when there's not enough memory
    FKAssert(newArr != 0, "fkArrayDoubleCapacity failed. Not enough memory");
	arr->arr = newArr;
}

void fkArrayEnsureExtraCapacity(fkArray *arr, unsigned int extra)
{
	while (arr->max < arr->num + extra)
    {
		fkArrayDoubleCapacity(arr);
    }
}

void fkArrayShrink(fkArray *arr)
{
    unsigned int newSize = 0;
	
	//only resize when necessary
	if (arr->max > arr->num && !(arr->num==0 && arr->max==1))
	{
		if (arr->num!=0)
		{
			newSize=arr->num;
			arr->max=arr->num;
		}
		else
		{//minimum capacity of 1, with 0 elements the array would be free'd by realloc
			newSize=1;
			arr->max=1;
		}
		
		arr->arr = (Object**)realloc(arr->arr,newSize * sizeof(Object*) );
		FKAssert(arr->arr!=NULL,"could not reallocate the memory");
	}
}

/** Returns index of first occurrence of object, CC_INVALID_INDEX if object not found. */
unsigned int fkArrayGetIndexOfObject(fkArray *arr, Object* object)
{
    const unsigned int arrNum = arr->num;
    Object** ptr = arr->arr;
	for(unsigned int i = 0; i < arrNum; ++i, ++ptr)
    {
		if( *ptr == object ) return i;
    }
    
	return FK_INVALID_INDEX;
}

/** Returns a Boolean value that indicates whether object is present in array. */
bool fkArrayContainsObject(fkArray *arr, Object* object)
{
	return fkArrayGetIndexOfObject(arr, object) != FK_INVALID_INDEX;
}

/** Appends an object. Behavior undefined if array doesn't have enough capacity. */
void fkArrayAppendObject(fkArray *arr, Object* object)
{
    FKAssert(object != NULL, "Invalid parameter!");
    object->retain();
	arr->arr[arr->num] = object;
	arr->num++;
}

/** Appends an object. Capacity of arr is increased if needed. */
void fkArrayAppendObjectWithResize(fkArray *arr, Object* object)
{
	fkArrayEnsureExtraCapacity(arr, 1);
	fkArrayAppendObject(arr, object);
}

/** Appends objects from plusArr to arr. Behavior undefined if arr doesn't have
 enough capacity. */
void fkArrayAppendArray(fkArray *arr, fkArray *plusArr)
{
	for(unsigned int i = 0; i < plusArr->num; i++)
    {
		fkArrayAppendObject(arr, plusArr->arr[i]);
    }
}

/** Appends objects from plusArr to arr. Capacity of arr is increased if needed. */
void fkArrayAppendArrayWithResize(fkArray *arr, fkArray *plusArr)
{
	fkArrayEnsureExtraCapacity(arr, plusArr->num);
	fkArrayAppendArray(arr, plusArr);
}

/** Inserts an object at index */
void fkArrayInsertObjectAtIndex(fkArray *arr, Object* object, unsigned int index)
{
	FKAssert(index<=arr->num, "Invalid index. Out of bounds");
	FKAssert(object != NULL, "Invalid parameter!");

	fkArrayEnsureExtraCapacity(arr, 1);
	
	unsigned int remaining = arr->num - index;
	if( remaining > 0)
    {
		memmove((void *)&arr->arr[index+1], (void *)&arr->arr[index], sizeof(Object*) * remaining );
    }

    object->retain();
	arr->arr[index] = object;
	arr->num++;
}

/** Swaps two objects */
void fkArraySwapObjectsAtIndexes(fkArray *arr, unsigned int index1, unsigned int index2)
{
	FKAssert(index1 < arr->num, "(1) Invalid index. Out of bounds");
	FKAssert(index2 < arr->num, "(2) Invalid index. Out of bounds");
	
	Object* object1 = arr->arr[index1];
	
	arr->arr[index1] = arr->arr[index2];
	arr->arr[index2] = object1;
}

/** Removes all objects from arr */
void fkArrayRemoveAllObjects(fkArray *arr)
{
	while( arr->num > 0 )
    {
		(arr->arr[--arr->num])->release();
    }
}

/** Removes object at specified index and pushes back all subsequent objects.
 Behavior undefined if index outside [0, num-1]. */
void fkArrayRemoveObjectAtIndex(fkArray *arr, unsigned int index, bool bReleaseObj/* = true*/)
{
    FKAssert(arr && arr->num > 0 && index < arr->num, "Invalid index. Out of bounds");
    if (bReleaseObj)
    {
        FK_SAFE_RELEASE(arr->arr[index]);
    }
    
	arr->num--;
	
	unsigned int remaining = arr->num - index;
	if(remaining>0)
    {
		memmove((void *)&arr->arr[index], (void *)&arr->arr[index+1], remaining * sizeof(Object*));
    }
}

/** Removes object at specified index and fills the gap with the last object,
 thereby avoiding the need to push back subsequent objects.
 Behavior undefined if index outside [0, num-1]. */
void fkArrayFastRemoveObjectAtIndex(fkArray *arr, unsigned int index)
{
	FK_SAFE_RELEASE(arr->arr[index]);
	unsigned int last = --arr->num;
	arr->arr[index] = arr->arr[last];
}

void fkArrayFastRemoveObject(fkArray *arr, Object* object)
{
	unsigned int index = fkArrayGetIndexOfObject(arr, object);
	if (index != FK_INVALID_INDEX)
    {
		fkArrayFastRemoveObjectAtIndex(arr, index);
    }
}

/** Searches for the first occurrence of object and removes it. If object is not
 found the function has no effect. */
void fkArrayRemoveObject(fkArray *arr, Object* object, bool bReleaseObj/* = true*/)
{
	unsigned int index = fkArrayGetIndexOfObject(arr, object);
	if (index != FK_INVALID_INDEX)
    {
		fkArrayRemoveObjectAtIndex(arr, index, bReleaseObj);
    }
}

/** Removes from arr all objects in minusArr. For each object in minusArr, the
 first matching instance in arr will be removed. */
void fkArrayRemoveArray(fkArray *arr, fkArray *minusArr)
{
	for(unsigned int i = 0; i < minusArr->num; i++)
    {
		fkArrayRemoveObject(arr, minusArr->arr[i]);
    }
}

/** Removes from arr all objects in minusArr. For each object in minusArr, all
 matching instances in arr will be removed. */
void fkArrayFullRemoveArray(fkArray *arr, fkArray *minusArr)
{
	unsigned int back = 0;
	unsigned int i = 0;
	
	for( i = 0; i < arr->num; i++) 
    {
		if( fkArrayContainsObject(minusArr, arr->arr[i]) ) 
        {
			FK_SAFE_RELEASE(arr->arr[i]);
			back++;
		} 
        else
        {
			arr->arr[i - back] = arr->arr[i];
        }
	}
	
	arr->num -= back;
}

// #pragma mark -
// #pragma mark ccCArray for Values (c structures)

/** Allocates and initializes a new C array with specified capacity */
fkCArray* fkCArrayNew(unsigned int capacity)
{
	if (capacity == 0)
    {
		capacity = 1;
    }

	fkCArray *arr = (fkCArray*)malloc( sizeof(fkCArray) );
	arr->num = 0;
	arr->arr = (void**)malloc( capacity * sizeof(void*) );
	arr->max = capacity;
	
	return arr;
}

/** Frees C array after removing all remaining values. Silently ignores NULL arr. */
void fkCArrayFree(fkCArray *arr)
{
    if( arr == NULL ) 
    {
        return;
    }
	fkCArrayRemoveAllValues(arr);
	
	free(arr->arr);
	free(arr);
}

/** Doubles C array capacity */
void fkCArrayDoubleCapacity(fkCArray *arr)
{
    fkArrayDoubleCapacity((fkArray*)arr);
}

/** Increases array capacity such that max >= num + extra. */
void fkCArrayEnsureExtraCapacity(fkCArray *arr, unsigned int extra)
{
    fkArrayEnsureExtraCapacity((fkArray*)arr,extra);
}

/** Returns index of first occurrence of value, CC_INVALID_INDEX if value not found. */
unsigned int fkCArrayGetIndexOfValue(fkCArray *arr, void* value)
{
	unsigned int i;
	
	for( i = 0; i < arr->num; i++)
    {
		if( arr->arr[i] == value ) return i;
    }
	return FK_INVALID_INDEX;
}

/** Returns a Boolean value that indicates whether value is present in the C array. */
bool fkCArrayContainsValue(fkCArray *arr, void* value)
{
	return fkCArrayGetIndexOfValue(arr, value) != FK_INVALID_INDEX;
}

/** Inserts a value at a certain position. Behavior undefined if array doesn't have enough capacity */
void fkCArrayInsertValueAtIndex( fkCArray *arr, void* value, unsigned int index)
{
	FKAssert( index < arr->max, "fkCArrayInsertValueAtIndex: invalid index");
	
	unsigned int remaining = arr->num - index;
    // make sure it has enough capacity
    if (arr->num + 1 == arr->max)
    {
        fkCArrayDoubleCapacity(arr);
    }
	// last Value doesn't need to be moved
	if( remaining > 0) {
		// tex coordinates
		memmove((void *)&arr->arr[index+1], (void *)&arr->arr[index], sizeof(void*) * remaining );
	}
	
	arr->num++;
	arr->arr[index] = value;
}

/** Appends an value. Behavior undefined if array doesn't have enough capacity. */
void fkCArrayAppendValue(fkCArray *arr, void* value)
{
	arr->arr[arr->num] = value;
	arr->num++;
    // double the capacity for the next append action
    // if the num >= max
    if (arr->num >= arr->max)
    {
        fkCArrayDoubleCapacity(arr);
    }
}

/** Appends an value. Capacity of arr is increased if needed. */
void fkCArrayAppendValueWithResize(fkCArray *arr, void* value)
{
	fkCArrayEnsureExtraCapacity(arr, 1);
	fkCArrayAppendValue(arr, value);
}


/** Appends values from plusArr to arr. Behavior undefined if arr doesn't have
 enough capacity. */
void fkCArrayAppendArray(fkCArray *arr, fkCArray *plusArr)
{
	unsigned int i;
	
	for( i = 0; i < plusArr->num; i++)
    {
		fkCArrayAppendValue(arr, plusArr->arr[i]);
    }
}

/** Appends values from plusArr to arr. Capacity of arr is increased if needed. */
void fkCArrayAppendArrayWithResize(fkCArray *arr, fkCArray *plusArr)
{
	fkCArrayEnsureExtraCapacity(arr, plusArr->num);
	fkCArrayAppendArray(arr, plusArr);
}

/** Removes all values from arr */
void fkCArrayRemoveAllValues(fkCArray *arr)
{
	arr->num = 0;
}

/** Removes value at specified index and pushes back all subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */
void fkCArrayRemoveValueAtIndex(fkCArray *arr, unsigned int index)
{
	unsigned int last;
	
	for( last = --arr->num; index < last; index++)
    {
		arr->arr[index] = arr->arr[index + 1];
    }
}

/** Removes value at specified index and fills the gap with the last value,
 thereby avoiding the need to push back subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */
void fkCArrayFastRemoveValueAtIndex(fkCArray *arr, unsigned int index)
{
	unsigned int last = --arr->num;
	arr->arr[index] = arr->arr[last];
}

/** Searches for the first occurrence of value and removes it. If value is not found the function has no effect.
 @since v0.99.4
 */
void fkCArrayRemoveValue(fkCArray *arr, void* value)
{
	unsigned int index = fkCArrayGetIndexOfValue(arr, value);
	if (index != FK_INVALID_INDEX)
    {
		fkCArrayRemoveValueAtIndex(arr, index);
    }
}

/** Removes from arr all values in minusArr. For each Value in minusArr, the first matching instance in arr will be removed.
 @since v0.99.4
 */
void fkCArrayRemoveArray(fkCArray *arr, fkCArray *minusArr)
{
	for(unsigned int i = 0; i < minusArr->num; i++)
    {
		fkCArrayRemoveValue(arr, minusArr->arr[i]);
    }
}

/** Removes from arr all values in minusArr. For each value in minusArr, all matching instances in arr will be removed.
 @since v0.99.4
 */
void fkCArrayFullRemoveArray(fkCArray *arr, fkCArray *minusArr)
{
	unsigned int back = 0;
	
	for(unsigned int i = 0; i < arr->num; i++) 
    {
		if( fkCArrayContainsValue(minusArr, arr->arr[i]) ) 
        {
			back++;
		} 
        else
        {
			arr->arr[i - back] = arr->arr[i];
        }
	}
	
	arr->num -= back;
}

FLAKOR_NS_END

