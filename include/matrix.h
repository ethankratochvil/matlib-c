#ifndef MATRIX_H
#define MATRIX_H

// Core matrix struct; Flat array layout.
typedef struct {
    int rows;
    int cols;
    double *data;
} Matrix;

// Matrix access macro.
#define MAT(m, i, j) ((m)->data[(i) * (m)->cols + (j)])

// Creates a new rows x cols matrix, zero-initialized.
// Returns NULL on allocation failure.
// Caller is responsible for calling matrix_free().
Matrix* matrix_create(int rows, int cols);

// Frees all memory associated with matrix m.
void matrix_free(Matrix *m);

// Prints the matrix to stdout in a readable format.
void matrix_print(const Matrix *m);

// Returns a deep copy of src matrix.
// Caller must call matrix_free().
Matrix* matrix_copy(const Matrix *src);

// Sets element at (i, j) to val. Bounds are checked.
void matrix_set(Matrix *m, int i, int j, double val);

// Returns element at (i, j) to val. Bounds are checked.
double matrix_get(const Matrix *m, int i, int j);

// Adds each element together. Dimensions are checked.
Matrix* matrix_add(const Matrix *m1, const Matrix *m2);

// Multiplies matrix by a scalar. Returns new matrix.
Matrix* matrix_scale(const Matrix *m, double scalar);

// Returns transposed matrix or NULL on failure.
Matrix* matrix_transpose(const Matrix *m);

// Multiplies two matrices. Dimensions are checked.
// Returns product matrix or NULL on failure.
Matrix* matrix_multiply(const Matrix *m1, const Matrix *m2);

// Returns identity matrix; square with 1's on diagonal.
// Returns NULL on failure.
Matrix* matrix_identity(int n);

#endif
