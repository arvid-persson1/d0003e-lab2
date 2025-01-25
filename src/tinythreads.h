#ifndef _TINYTHREADS_H
#define _TINYTHREADS_H

struct thread_block;
typedef struct thread_block *thread;
void spawn(void (*code)(int), int arg);
void yield(void);

typedef struct mutex {
    int locked;
    thread waitQ;
} mutex;

#define MUTEX_INIT {0, 0}
void lock(mutex *m);
void unlock(mutex *m);

#endif

