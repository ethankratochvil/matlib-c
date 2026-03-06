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

#endif
