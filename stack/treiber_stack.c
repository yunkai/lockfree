
/*
 * Copyright (C) Yunkai Zhang
 * Copyright (C) Alibaba Group Holding Limited
 */


#include "lfstack.h"
#include "lfcomm.h"

typedef volatile uint64_t atomic_t;

struct lfstack_t {
    uint64_t    head;
    size_t      offset;
};


void
lfstack_init(lfstack_t *l, size_t offset)
{
    l->head = 0;
    l->offset = offset;
}


void
lfstack_push(lfstack_t *l, void *item)
{
    int         rc;
    atomic_t    prev, curr, addr, ver, *target;

    target = OFFSET_ADDR(item, l->offset);

    do {
        prev = l->head;

        addr = PTR_ADDR_GET(prev);
        ver = PTR_ADDR_GET(prev);

        *target = addr;

        curr = (atomic_t)PTR_VER_SET(item, ver);

        rc = CAS(&l->head, prev, curr);
    } while (rc == 0);

    return;
}


void *
lfstack_pop(lfstack_t *l)
{
    int         rc;
    atomic_t    prev, next, addr, ver, *target;

    do {
        prev = l->head;

        addr = PTR_ADDR_GET(prev);

        if (addr == 0) {
            return NULL;
        }

        ver = PTR_ADDR_GET(prev);

        target = OFFSET_ADDR(addr, l->offset);

        next = (atomic_t)PTR_VER_SET(*target, ver + 1);

        rc = CAS(&l->head, prev, next);
    } while (rc == 0);

    *target = 0;
    return (void *) addr;
}


void *
lfstack_popall(lfstack_t *l)
{
    int         rc;
    atomic_t    prev, next, addr, ver;

    do {
        prev = l->head;

        addr = PTR_ADDR_GET(prev);

        if (addr == 0) {
            return NULL;
        }

        ver = PTR_ADDR_GET(prev);

        next = (atomic_t)PTR_VER_SET(0, ver + 1);

        rc = CAS(&l->head, prev, next);
    } while (rc == 0);

    return (void *) addr;
}


void *
lfstack_remove(lfstack_t *l, void *item)
{
    int         rc;
    atomic_t    prev, next, addr, ver, *target, *target_next;

retry:
    prev = l->head;

    addr = PTR_ADDR_GET(prev);

    target = OFFSET_ADDR(item, l->offset);

    while ((void *) addr == item) {
        ver = PTR_ADDR_GET(prev);

        next = (atomic_t)PTR_VER_SET(*target, ver + 1);

        rc = CAS(&l->head, prev, next);
        if (rc) {
            *target = 0;
            return item;
        }

        prev = l->head;

        addr = PTR_ADDR_GET(prev);
    }

    next = addr;

    while (next) {
        target_next = OFFSET_ADDR(next, l->offset);

        next = *target_next;

        if ((void *) next == item) {

            rc = CAS(target_next, next, *target);
            if (rc) {
                *target = 0;
                return item;
            }

            goto retry;
        }
    }

    return NULL;
}
