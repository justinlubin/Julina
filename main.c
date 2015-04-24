#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "julina.h"

/*
 * Format:
 * 3 2 : 1 2 3 4 5 6
 * Yields:
 * [[1 2]
 *  [3 4]
 *  [5 6]]
 */
Matrix *read_matrix(char *s) {
    int rows, cols, size;
    rows = strtol(s, &s, 10);
    cols = strtol(s, &s, 10);
    size = rows * cols;
    double aa[size];
    s += 3; // space colon space
    int i;
    for (i = 0; i < size; i++) {
        aa[i] = strtol(s, &s, 10);
    }
    return new_matrix(aa, rows, cols);
}

int main(int argc, char **argv) {
    srand(time(NULL));

    Matrix *a = read_matrix("3 3 : 3 2 1 1 -1 3 5 4 -2");
    Matrix *b = read_matrix("3 1 : 7 3 1");
    Matrix *x = cramer(a, b);

    print_matrix(a);
    print_matrix(b);
    print_matrix(x);

    free_matrix(a);
    free_matrix(b);
    free_matrix(x);
}
