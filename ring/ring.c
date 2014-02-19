#include "ring.h"
#include "stdlib.h"

struct ring_t {
	void **buf;
	int size;
	volatile int head;
	volatile int tail;
};

ring_t
ring_create(void *buf[], int size)
{
	ring_t r = malloc(sizeof(struct ring_t));

	r->buf = buf;
	r->size = size;


	//
	// 1) head and tail should always less than size.
	// 2) head == tail, means empty.
	// 3) (head + 1) % size == tail, means full.
	//
	r->head = 0;
	r->tail = 0;

	return r;
}

void
ring_destroy(ring_t r)
{
	free(r);
}

bool
ring_put(ring_t r, void *data)
{
	int idx;

	idx = (r->head + 1) % r->size;

	//
	// Is buffer full?
	//
	if (idx == r->tail)
		return false;

	r->buf[idx] = data;

	r->head = idx;

	return true;
}

bool
ring_get(ring_t r, void **data)
{
	int idx;

	//
	// Is buffer empty?
	//
	if (r->tail == r->head)
		return false;

	idx = r->tail;

	r->tail = (r->tail + 1) % r->size;

	*data = r->buf[idx];

	return true;
}

