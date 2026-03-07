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