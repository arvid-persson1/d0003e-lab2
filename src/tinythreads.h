#ifndef _TINYTHREADS_H
#define _TINYTHREADS_H

#include <stdbool.h>

struct ThreadBlock;
typedef struct ThreadBlock *Thread;
void spawn(void (*code)(int), int arg);
void yield(void);

typedef struct Mutex {
    bool locked;
    Thread waitQ;
} Mutex;

#define MUTEX_INIT {0, 0}
void lock(Mutex *m);
void unlock(Mutex *m);

#endif

