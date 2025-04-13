#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
	if (argc == 1) {
		printf("Usage: ./memeory-user [size]\n");
		return 0;
	}
	int size = atoi(argv[1]) * 1024 * 1024;
	char* arr = malloc(size);
	
	printf("%d\n", getpid());
	for (int j = 0; j < 10000000000; j++) {
		for (int i = 0; i < size; i++)
			arr[i] = 0;
		printf("%d\n", j);
	}

	free(arr);
	return 0;
}
