#include <stdio.h>
#include "julina.h"

int main(int argc, char **argv) {
    Matrix *Z = zero_matrix(3, 4);
    Matrix *I = identity_matrix(5);

    double ai[] = {1, 2, 3, 4, 5, 6};
    Matrix *a = new_matrix(ai, 2, 3);

    double bi[] = {2, 0, -1, 0, 6, 0, 0, -2, 0, 2, -2, -1};
    Matrix *b = new_matrix(bi, 3, 4);

    double ci[] = {0, 0, 0};
    Matrix *c = new_matrix(ci, 3, 1);
    
    Matrix *d = augment(b, c);
    Matrix *e = ref(d);
    Matrix *f = rref(d);

    print_matrix(Z);
    print_matrix(I);
    print_matrix(a);
    print_matrix(b);
    print_matrix(c);
    print_matrix(d);
    print_matrix(e);
    print_matrix(f);

    free_matrix(Z);
    free_matrix(I);
    free_matrix(a);
    free_matrix(b);
    free_matrix(c);
    free_matrix(d);
    free_matrix(e);
    free_matrix(f);
}
