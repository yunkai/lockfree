#ifndef _LOCKFREE_STACK_H_
#define _LOCKFREE_STACK_H_

#include "lfcomm.h"

typedef struct lfstack_t lfstack_t;

void lfstack_init(lfstack_t *l, size_t offset);
void lfstack_push(lfstack_t *l, void *item);
void *lfstack_pop(lfstack_t *l);
void *lfstack_popall(lfstack_t *l);
void *lfstack_remove(lfstack_t *l, void *item);


#endif /* _LOCKFREE_STACK_H_ */
