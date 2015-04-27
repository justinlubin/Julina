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

    long i;
    for (i = 0; i < 10000000000; i++) {
        Matrix *a = random_matrix(4, 5, -10, 10);
        int n;
        if ((n = rank(a)) < 3) {
            printf("yooo! %d \n", n);
            print_matrix(a);
        }
        free_matrix(a);
    }

    Matrix *xy = read_matrix("5 2 : 1 1 2 3 3 4 4 6 5 5");
    Matrix *x = least_squares(xy);

    print_matrix(xy);
    printf("Line of best fit: y = %gx + %g\n",
           x->array[0][0],
           x->array[1][0]);

    free_matrix(xy);
    free_matrix(x);
}
