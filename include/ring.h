#ifndef _RING_H_
#define _RING_H_

#include "stdbool.h"

typedef struct ring_t *ring_t;

//
// !!NOTE!!:
// This ring buffer should be only used in
// single producer/single consumer scenario.
//

//
// Create a ring buffer
// buf[]: an array that could contain at most
//        _size_ elements with void* type.
// size: the size of buf[] array. 
//
// return created ring buffer.
//
extern ring_t ring_create(void *buf[], int size);

//
// Put data into ring buffer
// r: ring buffer
// data: data to be put.
//
// return false if fail to put when buffer is full,
// else return true.
//
extern bool ring_put(ring_t r, void *data);


//
// Get data from ring buffer
// r: ring buffer
// data: data comes from ring buffer
//       if buffer is not empty.
//
// return false if buffer is empty,
// else return true.
//
extern bool ring_get(ring_t r, void **data);

//
// Destroy ring buffer
//
extern void ring_destroy(ring_t r);

#endif
