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
        aa[i] = strtof(s, &s);
    }
    return new_matrix(aa, rows, cols);
}

int main(int argc, char **argv) {
    srand(time(NULL));

    Matrix *xy = read_matrix("4 5 : 0 2 4 2 2 4 1 0 5 1 2 1 0.6666666666666666666666666666 3 .3333333333333333333333333333 6 6 6 12 0");
    xy->array[2][2] = 2.0/3;
    xy->array[2][4] = 1.0/3;

    int r = rank(xy);
    printf("Rank: %d\n", r);

    print_matrix(rref(xy));
}
