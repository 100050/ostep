#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>

#define COUNT 1000000
#define NUMCPU 16

typedef struct __counter_t {
    pthread_mutex_t lock;
    pthread_mutex_t local_lock[NUMCPU];
    int value;
    int local_value[NUMCPU];
    int threshold;
} counter_t;

typedef struct __thread_info_t {
    counter_t *counter;
    pthread_t thread;
} thread_info_t;

void init(counter_t *c, int threshold) {
    c->threshold = threshold;
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
    for (int i = 0; i < NUMCPU; i++) {
        pthread_mutex_init(&c->local_lock[i], NULL);
        c->local_value[i] = 0;
    }

}

void increment(counter_t *c, int threadID) {
    int cpu = threadID % NUMCPU;
    pthread_mutex_lock(&c->local_lock[cpu]);
    c->local_value[cpu]++;
    if (c->local_value[cpu] >= c->threshold) {
        pthread_mutex_lock(&c->lock);
        c->value += c->local_value[cpu];
        pthread_mutex_unlock(&c->lock);
        c->local_value[cpu] = 0;
    }
    pthread_mutex_unlock(&c->local_lock[cpu]);
}

int get(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    int tmp = c->value;
    pthread_mutex_unlock(&c->lock);
    return tmp;
}

void *thread_f(void *arg) {
    thread_info_t *tmp = (thread_info_t*)arg;   
    // COUNT 만큼 증가
    pthread_t threadID = pthread_self();
    for (int i = 0; i < COUNT; i++) {
        increment(tmp->counter, (int)threadID);
    }

    pthread_exit(EXIT_SUCCESS);
}

int main() {

    for (int i = 3; i <= 8; i++) { // threadhold
        double threshold = pow(2.0, (double)i);
        for (int j = 1; j <= NUMCPU; j++) { // 쓰레드 수
            struct timeval start, end;
            counter_t *counter = malloc(sizeof(counter_t));
            init(counter, (int)threshold);

            
            gettimeofday(&start, NULL);
            thread_info_t *thread_arr = malloc(j*sizeof(thread_info_t));
            for (int k = 0;  k < j; k++) {
                thread_arr[k].counter = counter;
                pthread_create(&thread_arr[k].thread, NULL, &thread_f, &thread_arr[k]);

            }

            for (int k = 0; k < j; k++) {
                pthread_join(thread_arr[k].thread, NULL);
            }

            gettimeofday(&end, NULL);
            
            // 결과 출력
            long long startusec, endusec;
            startusec = start.tv_sec * COUNT + start.tv_usec;
            endusec = end.tv_sec * COUNT + end.tv_usec;
            printf("%d threshold, %d threads\n", (int)threshold, j);
            printf("global count: %d\n", get(counter));
            printf("Time (seconds): %f\n\n", ((double)(endusec - startusec) / COUNT));

            pthread_mutex_destroy(&counter->lock);
            for (int i = 0; i < NUMCPU; i++) {
                pthread_mutex_destroy(&counter->local_lock[i]);
            }
            free(counter);
            free(thread_arr);
        }
    }
}