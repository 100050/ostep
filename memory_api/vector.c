#include <stdio.h>
#include <stdlib.h>


typedef struct _Vector {
	int *arr;
	int size;

} Vector;

Vector* init(int size, int val) {
	Vector* v = (Vector*)malloc(sizeof(Vector));
	v->arr = (int*)malloc(sizeof(int)*size);
	v->size = size;
	
	for (int i = 0; i < size; i++) {
		v->arr[i] = val;
	}

	return v;
} 

void fini(Vector* v) {
	free(v->arr);
	free(v);
}

void pushBack(Vector* v, int item) {
	v->arr = (int*)realloc(v->arr, sizeof(int)*(v->size+1));

	v->arr[v->size] = item;
	v->size++;
}

int main() {
	Vector* vec = init(5, 1);

	vec->arr[0] = 5;
	
	for (int i = 0; i < vec->size; i++) printf("%d\n", vec->arr[i]);

	pushBack(vec, 3);

	for (int i = 0; i < vec->size; i++) printf("%d\n", vec->arr[i]);

	fini(vec);
	return 0;
}
