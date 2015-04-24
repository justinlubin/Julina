#ifndef __JULINA_H__
#define __JULINA_H__

typedef struct {
    double **array;
    int rows, cols;
} Matrix;

Matrix *ERR_INVALID_SIZE;
Matrix *ERR_SINGULAR_MATRIX_INVERSE;
Matrix *ERR_DETERMINANT_ZERO;

Matrix *zero_matrix(int rows, int cols);
Matrix *identity_matrix(int n);
Matrix *new_matrix(double *array, int rows, int cols);
Matrix *random_matrix(int rows, int cols, int low, int high);
Matrix *copy_matrix(const Matrix *a);
Matrix *cramer_ak_matrix(const Matrix *a, const Matrix *b, int k);

void free_matrix(Matrix *a);
void print_matrix(const Matrix *a);

Matrix *add(const Matrix *a, const Matrix *b);
Matrix *scale(const Matrix *a, double c);
Matrix *multiply(const Matrix *a, const Matrix *b);
Matrix *transpose(const Matrix *a);

Matrix *augment(const Matrix *a, const Matrix *b);
Matrix *diminish_left(const Matrix *a, int cols);
Matrix *diminish_right(const Matrix *a, int cols);
int is_row_zero(const Matrix *a, int r);
int has_zero_row(const Matrix *a);
void swap_rows(Matrix *a, int r1, int r2);
void scale_row(Matrix *a, int r, double c);
void add_scaled_row(Matrix *a, int r1, int r2, double c);
Matrix *ref(const Matrix *a);
Matrix *rref(const Matrix *a);

int rank(const Matrix *a);
double det(const Matrix *a);

Matrix *inverse(const Matrix *a);

Matrix *cramer(const Matrix *a, const Matrix *b);

#endif
