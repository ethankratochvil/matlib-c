#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "matrix.h"

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

    double *m_data = (double*)malloc(sizeof(double) * (rows * cols));
    if (m_data == NULL) {
        fprintf(stderr, "matrix_create: memory allocation failed\n");
        free(m);
        return NULL;
    }

    m->rows = rows;
    m->cols = cols;
    m->data = m_data;

    for (int i = 0; i < (rows * cols); i++) {
        m->data[i] = 0;
    }

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