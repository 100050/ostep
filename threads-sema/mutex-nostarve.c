#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

typedef struct __ns_mutex_t {
    sem_t mutex;

} ns_mutex_t;

ns_mutex_t lock;

void ns_mutex_init(ns_mutex_t *m) {
    Sem_init(&m->mutex, 1);
}

void ns_mutex_acquire(ns_mutex_t *m) {
    Sem_wait(&m->mutex);
}

void ns_mutex_release(ns_mutex_t *m) {
    Sem_post(&m->mutex);
}


void *worker(void *arg) {
    for (int i = 0; i < 500; i++) {
        ns_mutex_acquire(&lock);
        printf("%d\n", (int)pthread_self());
        ns_mutex_release(&lock);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    printf("parent: begin\n");
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    pthread_t *threads = malloc(sizeof(pthread_t)*num_threads);

    ns_mutex_init(&lock);

    for (int i = 0; i < num_threads; i++)
        Pthread_create(&threads[i], NULL, &worker, NULL);
    
    for (int i = 0; i < num_threads; i++)
        Pthread_join(threads[i], NULL);
    
    printf("parent: end\n");

    free(threads);
    return 0;
}

