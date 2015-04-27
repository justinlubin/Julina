#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "julina.h"

#define MATRIX_SPACING 10
#define MATRIX_FORMAT "%-10.3g"

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
    return fabs(d) < ZERO_EPSILON;
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

Matrix *random_matrix(int rows, int cols, int low, int high) {
    int range = high - low + 1;
    int size = rows * cols;
    double random_array[size];
    int i;
    for (i = 0; i < size; i++) {
        random_array[i] = rand() % (range) + low;
    }
    return new_matrix(random_array, rows, cols);
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

Matrix *cramer_ak_matrix(const Matrix *a, const Matrix *b, int k) {
    if (a->rows != a->cols || a->rows != b->rows) {
        return ERR_INVALID_SIZE;
    }
    Matrix *c = copy_matrix(a);
    int i;
    for (i = 0; i < c->rows; i++) {
        c->array[i][k] = b->array[i][0];
    }
    return c;
}

Matrix *orthogonal_matrix(const Matrix *eigenvectors[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (eigenvectors[i]->rows != n && eigenvectors[i]->cols != 1) {
            return ERR_INVALID_BASIS;
        }
    }
    // [TODO] memory leak
    Matrix *a = copy_matrix(scale(eigenvectors[0], 1 / norm(eigenvectors[0])));
    Matrix *k;
    for (i = 1; i < n; i++) {
        k = scale(eigenvectors[i], 1 / norm(eigenvectors[i]));
        a = augment(a, k); // [TODO] MEMORY LEAK!!
    }
    return a;
}

void free_matrix(Matrix *a) {
    if (   a == ERR_INVALID_SIZE
        || a == ERR_SINGULAR_MATRIX_INVERSE) {
        return;
    }
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
        return ERR_INVALID_SIZE;
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

Matrix *subtract(const Matrix *a, const Matrix *b) {
    Matrix *neg_b = scale(b, -1);
    Matrix *sub = add(a, neg_b);
    free_matrix(neg_b);
    return sub;
}

Matrix *scale(const Matrix *a, double c) {
    Matrix *b = copy_matrix(a);
    int i, j;
    for (i = 0; i < b->rows; i++) {
        for (j = 0; j < b->cols; j++) {
            b->array[i][j] *= c;
        }
    }
    return b;
}

Matrix *multiply(const Matrix *a, const Matrix *b) {
    if (a->cols != b->rows) {
        return ERR_INVALID_SIZE;
    }
    int m = a->rows;
    int n = b->cols;
    int p = a->cols;

    Matrix *c = zero_matrix(m, n);

    int i, j, k;
    double sigma;
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
        return ERR_INVALID_SIZE;
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

Matrix *diminish_left(const Matrix *a, int cols) {
    Matrix *b = zero_matrix(a->rows, cols);
    int i, j;
    for (i = 0; i < b->rows; i++) {
        for (j = 0; j < b->cols; j++) {
            b->array[i][j] = a->array[i][j];
        }
    }
    return b;
}

Matrix *diminish_right(const Matrix *a, int cols) {
    Matrix *b = zero_matrix(a->rows, cols);
    int i, j;
    for (i = 0; i < b->rows; i++) {
        for (j = 0; j < b->cols; j++) {
            b->array[i][j] = a->array[i][a->cols - cols + j];
        }
    }
    return b;
}

int is_identity(const Matrix *a) {
    if (a->rows != a->cols) {
        return 0;
    }
    int i, j;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            if (i == j) {
                if (a->array[i][j] != 1) {
                    return 0;
                }
            } else {
                if (a->array[i][j] != 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
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

int has_zero_row(const Matrix *a) {
    int i;
    for (i = 0; i < a->rows; i++) {
        if (is_row_zero(a, i)) {
            return 1;
        }
    }
    return 0;
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
    int k, i;
    for (k = 0; k < b->rows; k++) {
        // Move zero rows to bottom of matrix.
        if (is_row_zero(b, k)) {
            for (i = k + 1; i < b->rows; i++) {
                if (!is_row_zero(b, i)) {
                    swap_rows(b, k, i);
                    break;
                }
            }
            // All remaining rows are zero.
            if (i == b->rows) {
                break;
            }
        }
        // Make sure we are operating on a row defined for position k.
        if (is_zero(b->array[k][k])) {
            for (i = k + 1; i < b->rows; i++) {
                if (!is_zero(b->array[i][k])) {
                    swap_rows(b, k, i);
                    break;
                }
            }
            // There is no valid row for position k.
            if (i == b->rows) {
                continue;
            }
        }
        // Scale position k to 1.
        scale_row(b, k, 1/b->array[k][k]);
        // Eliminate position k from all rows below.
        for (i = k + 1; i < b->rows; i++) {
            add_scaled_row(b, k, i, -b->array[i][k]);
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
        // Eliminate position k from all rows above.
        for (i = 0; i < k; i++) {
            add_scaled_row(b, k, i, -b->array[i][k]);
        }
    }
    return b;
}

int rank(const Matrix *a) {
    Matrix *b = ref(a);
    int i, nonzero_rows = 0;
    for (i = 0; i < b->rows; i++) {
        if (!is_row_zero(b, i)) {
            nonzero_rows += 1;
        }
    }
    free_matrix(b);
    return nonzero_rows;
}

double det(const Matrix *a) {
    if (a->rows != a->cols) {
        die("Error: determinant of non-square matrix.");
    } else if (a->rows == 2 && a->cols == 2) {
        return (a->array[0][0] * a->array[1][1] -
                a->array[0][1] * a->array[1][0]);
    }
    Matrix *b;
    double smaller[(a->rows - 1) * (a->cols - 1)];
    double determinant = 0;
    int sign = 1;

    int col, i, j, n;
    for (col = 0; col < a->cols; col++) {
        n = 0;
        for (i = 1; i < a->rows; i++) {
            for (j = 0; j < a->cols; j++) {
                if (j == col) {
                    continue;
                }
                smaller[n] = a->array[i][j];
                n += 1;
            }
        }
        b = new_matrix(smaller, a->rows - 1, a->cols - 1);
        determinant += sign * a->array[0][col] * det(b);
        sign *= -1;
        free_matrix(b);
    }

    return determinant;
}

Matrix *inverse(const Matrix *a) {
    if (a->rows != a->cols) {
        return ERR_INVALID_SIZE;
    }
    Matrix *id = identity_matrix(a->rows);
    Matrix *aug = augment(a, id);
    Matrix *flipped = rref(aug);
    Matrix *left = diminish_left(flipped, a->rows);
    Matrix *right = diminish_right(flipped, a->rows);

    free_matrix(id);
    free_matrix(aug);
    free_matrix(flipped);

    if (has_zero_row(left)) {
        return ERR_SINGULAR_MATRIX_INVERSE;
    }

    free_matrix(left);

    return right;
}

Matrix *cramer(const Matrix *a, const Matrix *b) {
    if (a->rows != a->cols || a->rows != b->rows) {
        return ERR_INVALID_SIZE;
    }
    double detA = det(a);
    if (is_zero(detA)) {
        return ERR_DETERMINANT_ZERO;
    }
    Matrix *x = zero_matrix(a->rows, 1);
    Matrix *ak;
    int k;
    for (k = 0; k < a->cols; k++) {
        ak = cramer_ak_matrix(a, b, k);
        x->array[k][0] = det(ak) / detA;
        free_matrix(ak);
    }
    return x;
}

Matrix *matrix_pow(const Matrix *a, int n) {
    if (a->rows != a->cols) {
        return ERR_INVALID_SIZE;
    } else if (n < 0) {
        return ERR_NEGATIVE_MATRIX_EXPONENT;
    } else if (n == 0) {
        return identity_matrix(a->rows);
    } else if (n == 1) {
        return copy_matrix(a);
    }
    Matrix *b = copy_matrix(a);
    while (n > 1) {
        if (n % 2 == 1) {
            // [TODO] call a plumber
            b = multiply(a, multiply(b, b));
            n -= 1;
            n /= 2;
        }
        if (n % 2 == 0) {
            b = multiply(b, b); // [TODO] memory leak
            n /= 2;
        }
    }
    return b;
}

double norm(const Matrix *a) {
    if (a->cols != 1) {
        die("Error: Invalid size matrix for norm.");
    }
    return sqrt(inner_product(a, a));
}

double inner_product(const Matrix *a, const Matrix *b) {
    if (a->cols != 1 || b->cols != 1 || a->rows != b->rows) {
        die("Error: Invalid size matrices for inner product.");
    }
    Matrix *at = transpose(a);
    Matrix *mul = multiply(at, b);
    double ip = mul->array[0][0];
    free_matrix(at);
    free_matrix(mul);
    return ip;
}

double dominant_eigenvalue(const Matrix *a, int m) {
    if (a->rows != a->cols) {
        die("Error: eigenvalue of non-square matrix.");
    }
    Matrix *x = zero_matrix(a->rows, 1);
    int i;
    for (i = 0; i < x->rows; i++) {
        x->array[i][0] = 1; // Initial guess
    }
    for (i = 0; i < m; i++) {
        x = multiply(a, x); // [TODO] memory leak
        x = scale(x, 1 / x->array[0][0]);
    }
    Matrix *ax = multiply(a, x);
    double numerator = inner_product(ax, x);
    double denominator = inner_product(x, x);
    free_matrix(ax);
    return numerator / denominator;
}

Matrix *get_eigenvector(const Matrix *a, double eigenvalue) {
    if (a->rows != a->cols) {
        die("Error: eigenvector of non-square matrix.");
    }
    Matrix *z = zero_matrix(a->rows, 1);
    Matrix *i = identity_matrix(a->rows);
    Matrix *li = scale(i, eigenvalue);
    Matrix *sub = subtract(a, li);
    Matrix *aug = augment(sub, z);

    Matrix *r1 = rref(aug);
    if (!is_zero(r1->array[r1->rows - 1][r1->cols - 2])) {
        return ERR_NOT_AN_EIGENVALUE;
    }

    // Final entry is, for example, 1.
    r1->array[r1->rows - 1][r1->cols - 1] = 1;
    r1->array[r1->rows - 1][r1->cols - 2] = 1;

    // Re-solve
    Matrix *r2 = rref(r1);

    Matrix *k = diminish_right(r2, 1);

    free_matrix(i);
    free_matrix(li);
    free_matrix(sub);
    free_matrix(r1);
    free_matrix(r2);

    k->array[k->rows - 1][0] = 1;
    return k;
}

Matrix *least_squares(const Matrix *xy) {
    if (xy->cols != 2) {
        return ERR_INVALID_SIZE;
    }
    Matrix *y = diminish_right(xy, 1);
    Matrix *a = copy_matrix(xy);
    int i;
    for (i = 0; i < a->rows; i++) {
        a->array[i][1] = 1;
    }
    Matrix *at = transpose(a);
    Matrix *ata = multiply(at, a);
    Matrix *atai = inverse(ata);
    Matrix *ataiat = multiply(atai, at);
    Matrix *x = multiply(ataiat, y);

    free_matrix(y);
    free_matrix(a);
    free_matrix(at);
    free_matrix(ata);
    free_matrix(atai);
    free_matrix(ataiat);

    return x;
}
