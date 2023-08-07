#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("No command line found\n");
        return 0;
    }

    int variable1 = atoi(argv[1]);
    int variable2 = atoi(argv[2]);

    printf("Before swapping:\n");
    printf("Variable1 = %d\n", variable1);
    printf("Variable2 = %d\n", variable2);

    swap(&variable1, &variable2);

    printf("After swapping:\n");
    printf("Variable1 = %d\n", variable1);
    printf("Variable2 = %d\n", variable2);

    return 0;
}