#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/un.h>
#include <time.h>

int **data;
int current;


void setter() {
	int *newV = (int*)calloc(5, sizeof(int));
	int i;
	for (i = 0; i < 5; i++) {
		newV[i] = rand();
	}
	data[current] = newV;
	current++;
}

void printer() {
	int i;
	for (i = 0; i < 5; i++) {
		printf("%i ", data[current][i]);
	}
	printf("\n");
	current++;
}

void main(int argc, char *argv[]) {
	printf("%i\n", argc);
	printf("%i\n", atoi(argv[1]));
	exit(0);
	int *vecLists[10];
	data = vecLists;
	current = 0;
	while(current < 10) {
		setter();
	}
	current = 0;
	while (current < 10) {
		printer();
	}
}