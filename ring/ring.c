#include "ring.h"
#include "stdlib.h"

struct ring_t {
  void **buf;
  int size;
  volatile int head;
  volatile int tail;
};

ring_t*
ring_new(void *buf[], int size)
{
  ring_t *r = malloc(sizeof(ring_t));

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
ring_free(ring_t *r)
{
  free(r);
}

bool
ring_put(ring_t *r, void *data)
{
  int idx;

  idx = (r->head + 1) % r->size;

  //
  // Is buffer full?
  //
  if (idx == r->tail)
    return false;

  r->buf[idx] = data;

  smp_wmb();

  r->head = idx;

  return true;
}

bool
ring_get(ring_t *r, void **data)
{
  int idx;

  idx = r->tail;

  //
  // Is buffer empty?
  //
  if (idx == r->head)
    return false;

  smp_read_barrier_depends();

  *data = r->buf[idx];

  r->tail = (r->tail + 1) % r->size;

  return true;
}

