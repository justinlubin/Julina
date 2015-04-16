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

    double gi[] = {2, 1, 1, 0, 3, 0, 3, 1, 2};
    Matrix *g = new_matrix(gi, 3, 3);

    Matrix *h = rref(g);

    double ii[] = {0, 0, 0, 0, 0, 3};
    Matrix *i = new_matrix(ii, 3, 2);

    Matrix *j = ref(i);

    print_matrix(Z);
    printf("Rank: %d\n\n", rank(Z));
    print_matrix(I);
    printf("Rank: %d\n\n", rank(I));
    print_matrix(a);
    printf("Rank: %d\n\n", rank(a));
    print_matrix(b);
    printf("Rank: %d\n\n", rank(b));
    print_matrix(c);
    printf("Rank: %d\n\n", rank(c));
    print_matrix(d);
    printf("Rank: %d\n\n", rank(d));
    print_matrix(e);
    printf("Rank: %d\n\n", rank(e));
    print_matrix(f);
    printf("Rank: %d\n\n", rank(f));
    print_matrix(g);
    printf("Rank: %d\n\n", rank(g));
    print_matrix(h);
    printf("Rank: %d\n\n", rank(h));
    print_matrix(i);
    printf("Rank: %d\n\n", rank(i));
    print_matrix(j);
    printf("Rank: %d\n\n", rank(j));

    free_matrix(Z);
    free_matrix(I);
    free_matrix(a);
    free_matrix(b);
    free_matrix(c);
    free_matrix(d);
    free_matrix(e);
    free_matrix(f);
    free_matrix(g);
    free_matrix(h);
    free_matrix(i);
    free_matrix(j);
}
