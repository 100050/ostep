#define _GNU_SOURCE 
#include <sched.h>

#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>


#define COUNT 1000000

typedef struct __counter_t {
    pthread_mutex_t lock;
    int value;
} counter_t;

typedef struct __thread_info_t {
    counter_t *counter;
    pthread_t thread;
    int cpu_idx;
} thread_info_t;

void init(counter_t *c) {
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
  }

void increment(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}

int get(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    int tmp = c->value;
    pthread_mutex_unlock(&c->lock);
    return tmp;
}

void *thread_f(void *arg) {
    thread_info_t *tmp = (thread_info_t*)arg; 
    cpu_set_t cpuset;
    // 특정 cpu에서 돌아가게 설정
    CPU_ZERO(&cpuset);
    CPU_SET(tmp->cpu_idx, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    // COUNT 만큼 증가
    for (int i = 0; i < COUNT; i++) {
        increment(tmp->counter);
    }

    pthread_exit(EXIT_SUCCESS);
}

int main() {
    int cpus = (int)sysconf(_SC_NPROCESSORS_ONLN), s;
    
    printf("cpu num: %d\n", cpus);

    for (int j = 1; j <= cpus; j++) { // 쓰레드 수
        for (int i = 1; i <= cpus; i++) { // cpu 수
            struct timeval start, end;
            counter_t *counter = malloc(sizeof(counter_t));
            init(counter);

            
            gettimeofday(&start, NULL);
            thread_info_t *thread_arr = malloc(j*sizeof(thread_info_t));
            for (int k = 0;  k < j; k++) {
                thread_arr[k].cpu_idx = k % i;
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
            printf("%d cpus, %d threads\n", i, j);
            printf("global count: %d\n", get(counter));
            printf("Time (seconds): %f\n\n", ((double)(endusec - startusec) / COUNT));

            pthread_mutex_destroy(&counter->lock);
            free(counter);
            free(thread_arr);
        }
    }
}