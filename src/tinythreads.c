#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "tinythreads.h"

#define NULL            0
#define DISABLE()       cli()
#define ENABLE()        sei()
#define STACKSIZE       80
#define NTHREADS        4
#define SETSTACK(buf, a) *((unsigned int *)(buf)+8) = (unsigned int)(a) + STACKSIZE - 4; \
                         *((unsigned int *)(buf)+9) = (unsigned int)(a) + STACKSIZE - 4

struct thread_block {
    void (*function)(int);
    int arg;
    thread next;
    jmp_buf context;
    char stack[STACKSIZE];
};

struct thread_block threads[NTHREADS],
                    initp;

thread freeQ   = threads,
       readyQ  = NULL,
       current = &initp;

bool initialized = false;

static void initialize(void) {
    int i = 0;

    while (i < NTHREADS) {
        threads[i].next = &threads[i + 1];
        i++;
    }

    threads[NTHREADS - 1].next = NULL;
    initialized = true;
}

static void enqueue(thread p, thread *queue) {
    p->next = NULL;

    if (*queue == NULL) {
        *queue = p;
    } else {
        thread q = *queue;
        while (q->next)
            q = q->next;
        q->next = p;
    }
}

static thread dequeue(thread *queue) {
    thread p = *queue;

    if (*queue) {
        *queue = (*queue)->next;
    } else {
        // Empty queue-- panic.
        while (true);
    }

    return p;
}

static void dispatch(thread next) {
    if (setjmp(current->context) == 0) {
        current = next;
        longjmp(next->context, 1);
    }
}

void spawn(void (* function)(int), int arg) {
    thread newp;

    DISABLE();

    if (!initialized)
        initialize();

    newp = dequeue(&freeQ);
    newp->function = function;
    newp->arg = arg;
    newp->next = NULL;

    if (setjmp(newp->context) == 1) {
        ENABLE();
        current->function(current->arg);
        DISABLE();
        enqueue(current, &freeQ);
        dispatch(dequeue(&readyQ));
    }

    SETSTACK(&newp->context, &newp->stack);

    enqueue(newp, &readyQ);

    ENABLE();
}

void yield(void) {

}

void lock(mutex *m) {

}

void unlock(mutex *m) {

}
