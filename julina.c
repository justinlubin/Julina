#include <stdlib.h>
#include <stdio.h>
#include "julina.h"

#define MATRIX_SPACING 8
#define MATRIX_FORMAT "%-8.3g"

#define ZERO_EPSILON 0.000000001

/* HELPER METHODS */

static void die(const char *message) {
    perror(message);
    exit(1);
}

static void repeat_print(const char *s, int n) {
    while (n--) {
        printf("%s", s);
    }
}


static int is_zero(double d) {
    return abs(d) < ZERO_EPSILON;
}

/* API */

Matrix *zero_matrix(int rows, int cols) {
    Matrix *a = malloc(sizeof (Matrix));
    if (!a) {
        die("Matrix could not be allocated.");
    }
    a->rows = rows;
    a->cols = cols;
    a->array = malloc(sizeof (a->array) * rows);
    if (!a->array) {
        die("Matrix array could not be allocated.");
    }
    int i;
    for (i = 0; i < rows; i++) {
        a->array[i] = calloc(cols, sizeof (a->array[i]));
        if (!a->array[i]) {
            die("Matrix row could not be allocated.");
        }
    }
    return a;
}

Matrix *identity_matrix(int n) {
    Matrix *a = zero_matrix(n, n);
    int i;
    for (i = 0; i < n; i++) {
        a->array[i][i] = 1;
    }
    return a;
}

Matrix *new_matrix(double *array, int rows, int cols) {
    Matrix *a = zero_matrix(rows, cols);
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            a->array[i][j] = array[i * cols + j];
        }
    }
    return a;
}

Matrix *copy_matrix(const Matrix *a) {
    Matrix *b = zero_matrix(a->rows, a->cols);
    int i, j;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            b->array[i][j] = a->array[i][j];
        }
    }
    return b;
}

void free_matrix(Matrix *a) {
    int i;
    for (i = 0; i < a->rows; i++) {
        free(a->array[i]);
    }
    free(a->array);
    free(a);
}

void print_matrix(const Matrix *a) {
    printf("┌");
    repeat_print(" ", MATRIX_SPACING * a->cols);
    printf("┐\n");

    int i, j;
    for (i = 0; i < a->rows; i++) {
        printf("|");
        for (j = 0; j < a->cols; j++) {
            // The extra ternary is so that -0 will not be displayed.
            printf(MATRIX_FORMAT, a->array[i][j] == 0 ? 0 : a->array[i][j]);
        }
        printf("|\n");
    }

    printf("└");
    repeat_print(" ", MATRIX_SPACING * a->cols);
    printf("┘\n");
}

Matrix *add(const Matrix *a, const Matrix *b) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return NULL;
    }
    Matrix *c = zero_matrix(a->rows, a->cols);
    int i, j;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            c->array[i][j] = a->array[i][j] + b->array[i][j];
        }
    }
    return c;
}

Matrix *multiply(const Matrix *a, const Matrix *b) {
    if (a->cols != b->rows) {
        return NULL;
    }
    int m = a->rows;
    int n = b->cols;
    int p = a->cols;

    Matrix *c = zero_matrix(m, n);

    int i, j, k, sigma;
    for (i = 0; i < c->rows; i++) {
        for (j = 0; j < c->cols; j++) {
            sigma = 0;
            for (k = 0; k < p; k++) {
                sigma += a->array[i][k] * b->array[k][j];
            }
            c->array[i][j] = sigma;
        }
    }

    return c;
}

Matrix *transpose(const Matrix *a) {
    Matrix *at = zero_matrix(a->cols, a->rows);
    int i, j;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            at->array[j][i] = a->array[i][j];
        }
    }
    return at;
}

Matrix *augment(const Matrix *a, const Matrix *b) {
    if (a->rows != b->rows) {
        return NULL;
    }
    Matrix *c = zero_matrix(a->rows, a->cols + b->cols);
    int i, j;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            c->array[i][j] = a->array[i][j];
        }
        for (j = 0; j < b->cols; j++) {
            c->array[i][a->cols + j] = b->array[i][j];
        }
    }
    return c;
}

int is_row_zero(const Matrix *a, int r) {
    int j;
    for (j = 0; j < a->cols; j++) {
        if (a->array[r][j] != 0) {
            return 0;
        }
    }
    return 1;
}

void swap_rows(Matrix *a, int r1, int r2) {
    double *temp_row = a->array[r1];
    a->array[r1] = a->array[r2];
    a->array[r2] = temp_row;
}

void scale_row(Matrix *a, int r, double c) {
    int j;
    for (j = 0; j < a->cols; j++) {
        a->array[r][j] *= c;
    }
}

void add_scaled_row(Matrix *a, int r1, int r2, double c) {
    int j;
    for (j = 0; j < a->cols; j++) {
        a->array[r2][j] += a->array[r1][j] * c;
    }
}

Matrix *ref(const Matrix *a) {
    Matrix *b = copy_matrix(a);
    int k, i, valid;
    for (k = 0; k < b->rows - 1; k++) {
        if (is_zero(b->array[k][k])) {
            valid = 0;
            for (i = k + 1; i < b->rows; i++) {
                if (!is_row_zero(b, i)) {
                    swap_rows(b, k, i);
                }
            }
            if (!valid) {
                // All the remaining rows are full of zeros.
                break;
            }
        }
        for (i = k + 1; i < b->rows; i++) {
            add_scaled_row(b, k, i, -b->array[i][k] / b->array[k][k]);
        }
    }
    return b;
}

Matrix *rref(const Matrix *a) {
    Matrix *b = ref(a);
    int k, i;
    for (k = b->rows - 1; k >= 0; k--) {
        if (is_zero(b->array[k][k])) {
            continue;
        }
        scale_row(b, k, 1/b->array[k][k]);
        for (i = 0; i < k; i++) {
            add_scaled_row(b, k, i, -b->array[i][k]);
        }
    }
    return b;
}
