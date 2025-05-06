#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>

typedef struct __node_t {
    struct __node_t *next;
    int value;
    pthread_mutex_t lock;
} node_t;

typedef struct __list_t {
    node_t *head;
    pthread_mutex_t insert_lock;
} list_t;

void init(list_t* list) {
    list->head = NULL;
    pthread_mutex_init(&list->insert_lock, NULL);
}

void insert(list_t* list, int value) {
    node_t *new = malloc(sizeof(node_t));
    new->value = value;
    pthread_mutex_init(&new->lock, NULL);
    
    pthread_mutex_lock(&list->insert_lock);
    new->next = list->head;
    list->head = new;
    pthread_mutex_unlock(&list->insert_lock);
    
}

int is_in(list_t* list, int value) {
    node_t* curr = list->head;
    if (!curr) return -1;
    int res = 0;

    pthread_mutex_lock(&curr->lock);
    while (curr) {
        if (curr->value == value) {
            res = 1;
            pthread_mutex_unlock(&curr->lock);
            break;
        }
        node_t *temp = curr;
        curr = curr->next;
        if (curr)
            pthread_mutex_lock(&curr->lock);
        pthread_mutex_unlock(&temp->lock);
    }

    return res;
}

void print(list_t* list) {
    node_t* curr = list->head;
    if (!curr) return;
    
    printf("list: ");
    pthread_mutex_lock(&curr->lock);
    while (curr) {
        printf("%d ", curr->value);
        node_t *temp = curr;
        curr = curr->next;
        if (curr)
            pthread_mutex_lock(&curr->lock);
        pthread_mutex_unlock(&temp->lock);
    }
    printf("\n");
}

void list_free(list_t* list) {
    node_t* curr = list->head;
    if (!curr) {
        free(list);
        return;
    }
    pthread_mutex_lock(&curr->lock);
    while (curr) {
        node_t *temp = curr;
        curr = curr->next;
        if (curr)
            pthread_mutex_lock(&curr->lock);
        pthread_mutex_unlock(&temp->lock);
        pthread_mutex_destroy(&temp->lock);
        free(temp);
    }
    free(list);
}

void *thread_f(void *arg) {
    list_t *l = (list_t*)arg;

    is_in(l, 0);
    //printf("%d: is value 0 in? %d\n", (int)pthread_self(), is_in(l, 0));

    pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s list_length threads print\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    list_t* list = malloc(sizeof(list_t));

    int list_length = atoi(argv[1]);
    int thread_count = atoi(argv[2]);
    int p = atoi(argv[3]);


    for (int i = 0; i < list_length; i++) {
        insert(list, i);
    }
    for (int i = 1; i <= thread_count; i++) {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        pthread_t *threads = malloc(sizeof(pthread_t)*i);
        
        for (int j  = 0; j < i; j++) {
            pthread_create(&threads[j], NULL, &thread_f, list);
        }
        
        for (int j  = 0; j < i; j++) {
            pthread_join(threads[j], NULL);
        }

        if (p) {
            print(list);
        }
        gettimeofday(&end, NULL);
        long long startusec, endusec;
        startusec = start.tv_sec * 1000000 + start.tv_usec;
        endusec = end.tv_sec * 1000000 + end.tv_usec;
        
        printf("%d threads, time (seconds): %f\n\n", i,
                ((double)(endusec - startusec) / 1000000));
        free(threads);
    }

    pthread_mutex_destroy(&list->insert_lock);
    list_free(list);
}