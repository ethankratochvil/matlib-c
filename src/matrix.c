#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "matrix.h"

#define EPSILON 1e-9

static void row_swap(Matrix *m, int r1, int r2) {
    if (m == NULL) return;
    else if (r1 == r2) return;
    else if (r1 < 0 || r1 > m->rows - 1 || r2 < 0 || r2 > m->rows - 1) return;

    double temp;
    for (int i = 0; i < m->cols; i++) {
        temp = MAT(m, r1, i);
        MAT(m, r1, i) = MAT(m, r2, i);
        MAT(m, r2, i) = temp;
    }
}

static void row_scale(Matrix *m, int row, double scalar) {
    if (m == NULL) return;
    else if (row < 0 || row > m->rows - 1) return;

    for (int i = 0; i < m->cols; i++) {
        MAT(m, row, i) *= scalar;
    }
}

static void row_add_scaled(Matrix *m, int target, int source, double scalar) {
    if (m == NULL) return;
    else if (target == source) return;
    else if (target < 0 || target > m->rows - 1 || source < 0 || source > m->rows - 1) return;
    else if (scalar == 0.0) return;

    for (int i = 0; i < m->cols; i++) {
        MAT(m, target, i) += scalar * MAT(m, source, i);
    }
}

static double det_2x2(const Matrix *m) {
    if (m == NULL) return 0.0;
    if (!matrix_is_square(m) || (m->rows != 2)) return 0.0;

    return MAT(m, 0, 0) * MAT(m, 1, 1) - MAT(m, 0, 1) * MAT(m, 1, 0);
}

Matrix* matrix_create(int rows, int cols) {
    if (rows < 1 || cols < 1) {
        fprintf(stderr, "matrix_create: invalid dimensions\n");
        return NULL;
    }

    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    if (m == NULL) {
        fprintf(stderr, "matrix_create: memory allocation failed\n");
        return NULL;
    }

    double *m_data = calloc((size_t)rows * cols, sizeof(*m_data));
    if (m_data == NULL) {
        fprintf(stderr, "matrix_create: memory allocation failed\n");
        free(m);
        return NULL;
    }

    m->rows = rows;
    m->cols = cols;
    m->data = m_data;

    return m;
}

void matrix_free(Matrix *m) {
    if (m == NULL) {
        fprintf(stderr, "matrix_free: *m was NULL\n");
        return;
    }

    free(m->data);
    free(m);
}

void matrix_print(const Matrix *m) {
    if (m == NULL) {
        fprintf(stderr, "matrix_print: *m was NULL\n");
        return;
    }

    for (int i = 0; i < m->rows; i++) {
        printf("[ ");
        for (int j = 0; j < m->cols; j++) {
            printf("%f ", MAT(m, i, j));
        }
        printf("]\n");
    }
}

Matrix* matrix_copy(const Matrix *src) {
    if (src == NULL) {
        fprintf(stderr, "matrix_copy: *src was NULL\n");
        return NULL;
    }

    Matrix *copy = (Matrix*)malloc(sizeof(Matrix));
    if (copy == NULL) {
        fprintf(stderr, "matrix_copy: memory allocation failed\n");
        return NULL;
    }
    
    double *copy_data = (double*)malloc(sizeof(double) * (src->rows * src->cols));
    if (copy_data == NULL) {
        fprintf(stderr, "matrix_copy: memory allocation failed\n");
        free(copy);
        return NULL;
    }

    copy->rows = src->rows;
    copy->cols = src->cols;
    copy->data = copy_data;

    memcpy(copy->data, src->data, sizeof(double) * (src->rows * src->cols));

    return copy;
}

void matrix_set(Matrix *m, int i, int j, double val) {
    if (m == NULL) {
        fprintf(stderr, "matrix_set: *m was NULL\n");
        return;
    }

    if (i < 0 || i > m->rows - 1 || j < 0 || j > m->cols - 1) {
        fprintf(stderr, "matrix_set: index out of bounds (%d, %d)\n", i, j);
        return;
    }

    MAT(m, i, j) = val;
}

double matrix_get(const Matrix *m, int i, int j) {
    if (m == NULL) {
        fprintf(stderr, "matrix_get: *m was NULL\n");
        return 0;
    }

    if (i < 0 || i > m->rows - 1 || j < 0 || j > m->cols - 1) {
        fprintf(stderr, "matrix_get: index out of bounds (%d, %d)\n", i, j);
        return 0;
    }

    return MAT(m, i, j);
}

Matrix* matrix_add(const Matrix *m1, const Matrix *m2) {
    if (m1 == NULL || m2 == NULL) {
        fprintf(stderr, "matrix_add: passed NULL\n");
        return NULL;
    }

    if (m1->rows != m2->rows || m1->cols != m2->cols) {
        fprintf(stderr, "matrix_add: dimensions do not match\n");
        return NULL;
    }

    Matrix *m = matrix_create(m1->rows, m1->cols);
    if (m == NULL) return NULL;

    for (int i = 0; i < (m->rows * m->cols); i++) {
        m->data[i] = m1->data[i] + m2->data[i];
    }

    return m;
}

Matrix* matrix_subtract(const Matrix *m1, const Matrix *m2) {
    if (m1 == NULL || m2 == NULL) {
        fprintf(stderr, "matrix_subtract: passed NULL\n");
        return NULL;
    }

    if (m1->rows != m2->rows || m1->cols != m2->cols) {
        fprintf(stderr, "matrix_subtract: dimensions do not match\n");
        return NULL;
    }

    Matrix *m = matrix_create(m1->rows, m1->cols);
    if (m == NULL) return NULL;

    for (int i = 0; i < (m->rows * m->cols); i++) {
        m->data[i] = m1->data[i] - m2->data[i];
    }

    return m;
}

Matrix* matrix_scale(const Matrix *m, double scalar) {
    if (m == NULL) {
        fprintf(stderr, "matrix_scale: passed NULL\n");
        return NULL;
    }

    Matrix *scaled_m = matrix_copy(m);
    if (scaled_m == NULL) return NULL;

    for (int i = 0; i < (scaled_m->rows * scaled_m->cols); i++) {
        scaled_m->data[i] *= scalar;
    }

    return scaled_m;
}

Matrix* matrix_transpose(const Matrix *m) {
    if (m == NULL) {
        fprintf(stderr, "matrix_transpose: passed NULL\n");
        return NULL;
    }

    Matrix *m_transpose = matrix_create(m->cols, m->rows);
    if (m_transpose == NULL) return NULL;

    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            MAT(m_transpose, j, i) = MAT(m, i, j);
        }
    }

    return m_transpose;
}

Matrix* matrix_multiply(const Matrix *m1, const Matrix *m2) {
    if (m1 == NULL || m2 == NULL) {
        fprintf(stderr, "matrix_multiply: passed NULL\n");
        return NULL;
    }

    if (m1->cols != m2->rows) {
        fprintf(stderr, "matrix_multiply: dimensions do not agree\n");
        return NULL;
    }

    Matrix *m_product = matrix_create(m1->rows, m2->cols);
    if (m_product == NULL) return NULL;

    double m1_val; // value in m1 being multiplied

    for (int i = 0; i < m_product->rows; i++) {
        for (int k = 0; k < m1->cols; k++) {
            m1_val = MAT(m1, i, k);

            for (int j = 0; j < m_product->cols; j++) {
                MAT(m_product, i, j) += m1_val * MAT(m2, k, j);
            }
        }
    }

    return m_product;
}

Matrix* matrix_identity(int n) {
    if (n < 1) {
        fprintf(stderr, "matrix_identity: impossible dimensions\n");
        return NULL;
    }

    Matrix *m_identity = matrix_create(n, n);
    if (m_identity == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        MAT(m_identity, i, i) = 1;
    }

    return m_identity;
}

int matrix_is_square(const Matrix *m) {
    if (m == NULL) {
        fprintf(stderr, "matrix_is_square: passed NULL\n");
        return 0;
    }

    if (m->rows == m->cols) return 1;
    else return 0;
}

double matrix_trace(const Matrix *m) {
    if (m == NULL) {
        fprintf(stderr, "matrix_trace: passed NULL\n");
        return 0;
    }

    if (!matrix_is_square(m)) {
        fprintf(stderr, "matrix_trace: matrix is not square\n");
        return 0;
    }

    double trace = 0;

    for (int i = 0; i < m->rows; i++) {
        trace += MAT(m, i, i);
    }

    return trace;
}

Matrix* matrix_rref(const Matrix *m) {
    if (m == NULL) {
        fprintf(stderr, "matrix_rref: passed NULL\n");
        return NULL;
    }

    Matrix* m_rref = matrix_copy(m);
    if (m_rref == NULL) return NULL;

    int pivot_row, row, col;
    double pivot_val;

    // tracking which column each pivot landed in
    int *pivot_cols = (int*)malloc(sizeof(int) * m_rref->rows);
    for (int i = 0; i < m_rref->rows; i++) pivot_cols[i] = -1;

    for (col = 0, row = 0; col < m_rref->cols && row < m_rref->rows; col++) {
        // Finding pivot, otherwise assume it's in current row
        pivot_row = row;
        pivot_val = 0.0;

        for (int i = row; i < m_rref->rows; i++) {
            if (fabs(MAT(m_rref, i, col)) > fabs(pivot_val)) {
                pivot_val = MAT(m_rref, i, col);
                pivot_row = i;
            }
        }

        if(fabs(pivot_val) < EPSILON) continue;

        row_swap(m_rref, row, pivot_row);
        row_scale(m_rref, row, 1.0 / MAT(m_rref, row, col));

        // Gaussian elimination
        for (int i = row + 1; i < m_rref->rows; i++) {
            row_add_scaled(m_rref, i, row, -MAT(m_rref, i, col));
        }

        pivot_cols[row] = col;
        row++;
    }

    for (int i = row - 1; i > 0; i--) {
        int pivot_col = pivot_cols[i];
        if (pivot_col == -1) continue; // no pivot in this row

        for (int k = i - 1; k >= 0; k--) {
            row_add_scaled(m_rref, k, i, -MAT(m_rref, k, pivot_col));
        }
    }

    free(pivot_cols);
    return m_rref;    
}

double matrix_det(const Matrix *m) {
    if (m == NULL) {
        fprintf(stderr, "matrix_det: passed NULL\n");
        return 0.0;
    }

    if (!matrix_is_square(m)) {
        fprintf(stderr, "matrix_det: matrix is not square\n");
        return 0.0;
    }

    // 2x2 matrix shortcut
    if (m->rows == 2) {
        return det_2x2(m);
    }


    Matrix* m_det = matrix_copy(m);
    if (m_det == NULL) return 0.0;

    int pivot_row, row, col;
    int swaps = 0;
    double pivot_val;

    for (col = 0, row = 0; col < m_det->cols && row < m_det->rows; col++) {
        // Finding pivot, otherwise assume it's in current row
        pivot_row = row;
        pivot_val = 0.0;

        for (int i = row; i < m_det->rows; i++) {
            if (fabs(MAT(m_det, i, col)) > fabs(pivot_val)) {
                pivot_val = MAT(m_det, i, col);
                pivot_row = i;
            }
        }

        if(fabs(pivot_val) < EPSILON) continue;

        if (pivot_row != row) {
            row_swap(m_det, row, pivot_row);
            swaps++; // Keeping track of swaps for (-1)^(swaps)
        }

        // Gaussian elimination
        for (int i = row + 1; i < m_det->rows; i++) {
            // Have to divide by value in pivot spot in scale because they are not scaled to 1
            row_add_scaled(m_det, i, row, -MAT(m_det, i, col) / MAT(m_det, row, col));
        }

        row++;
    }

    double diag_prod = 1; // Product of diagonal product of echelon matrix
    for (int i = 0; i < m_det->rows; i++) {
        diag_prod *= MAT(m_det, i, i);
    }

    matrix_free(m_det);

    return diag_prod * (swaps % 2 == 0 ? 1 : -1);
}