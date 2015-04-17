#include <stdio.h>
#include "julina.h"

int main(int argc, char **argv) {
    double ai[] = {2, -3, 1, 4};
    Matrix *a = new_matrix(ai, 2, 2);

    double bi[] = {-2, 5};
    Matrix *b = new_matrix(bi, 2, 1);

    double ci[] = {-1, 6, -2, 3};
    Matrix *c = new_matrix(ci, 2, 2);

    double di[] = {-7, 2};
    Matrix *d = new_matrix(di, 2, 1);

    Matrix *ret = add(multiply(a, b), scale(multiply(c, d), -1));

    print_matrix(ret);

    free_matrix(ret);
}
