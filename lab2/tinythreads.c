#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "tinythreads.h"

#define NULL            0
#define DISABLE()       cli()
#define ENABLE()        sei()
#define STACKSIZE       80
#define NTHREADS        4
#define SETSTACK(buf,a) *((unsigned int *)(buf)+8) = (unsigned int)(a) + STACKSIZE - 4; \
                        *((unsigned int *)(buf)+9) = (unsigned int)(a) + STACKSIZE - 4

struct thread_block {
    void (*function)(int);   // code to run
    int arg;                 // argument to the above
    thread next;             // for use in linked lists
    jmp_buf context;         // machine state
    char stack[STACKSIZE];   // execution stack space
};

struct thread_block threads[NTHREADS];

struct thread_block initp;

thread freeQ   = threads;
thread readyQ  = NULL;
thread current = &initp;

int initialized = 0;

static void initialize(void) {
    int i;
    for (i=0; i<NTHREADS-1; i++)
        threads[i].next = &threads[i+1];
    threads[NTHREADS-1].next = NULL;


    // Joystick stuff
    //PORTB = 0b10000000;
    //EIMSK = 0b10000000;
    //PCMSK1 = 0b10000000;
    
    // Timer stuff
             // Prescaler               // CTC
    TCCR1B = (1 << CS12) | (1 << CS10) | (1 << WGM12);
    TCNT1 = 0; // Reset timer
    
             // OC1A high on compare match
    TCCR1A = (1 << COM1A1) | (1 << COM1A0);

    OCR1A = 390; //(8Mhz / 1024) * 0.05
    TIMSK1 = (1 << ICIE1) | (1 << OCIE1A);

    PORTB = (1 << PB5);

    initialized = 1;
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
        // Empty queue, kernel panic!!!
        while (1) ;  // not much else to do...
    }
    return p;
}

static void dispatch(thread next) {
    if (setjmp(current->context) == 0) {
        current = next;
        longjmp(next->context,1);
    }
}

void spawn(void (* function)(int), int arg) {
    thread newp;

    DISABLE();
    if (!initialized) initialize();

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
    enqueue(current, &readyQ);
    dispatch(dequeue(&readyQ));
}

ISR(TIMER1_COMPA_vect) {
    yield();
}

void lock(mutex *m) {
    if (m->locked == 0){
        m->locked = 1;
    }

    else{
        enqueue(current, &(m->waitQ));
        dispatch(dequeue(&readyQ));
    }
}

void unlock(mutex *m) {
    if (m->waitQ != NULL){
        enqueue(current, &readyQ);
        dispatch(dequeue(&(m->waitQ)));
    }

    else {
        m->locked = 0;
    }
}
