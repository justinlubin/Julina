#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "julina.h"

int main(int argc, char **argv) {
    srand(time(NULL));

    double aa[] = {2, 0, 1,
                   -2, 3, 4,
                   -5, 5, 6};
    Matrix *a = new_matrix(aa, 3, 3);
    Matrix *ain = inverse(a);

    double bb[] = {1, -1, -2,
                   2, 4, 5,
                   6, 0, -3};
    Matrix *b = new_matrix(bb, 3, 3);
    Matrix *bin = inverse(b);

    print_matrix(a);
    if (ain == ERR_SINGULAR_MATRIX_INVERSE) {
        printf("Inverse of singular matrix.\n");
    } else {
        print_matrix(ain);
    }

    print_matrix(b);
    if (bin == ERR_SINGULAR_MATRIX_INVERSE) {
        printf("Inverse of singular matrix.\n");
    } else {
        print_matrix(bin);
    }

    free_matrix(a);
    free_matrix(ain);
    free_matrix(b);
    free_matrix(bin);
}
