#include <stdio.h>
#include <math.h>
#include "matrix.h"

#define EPSILON 0.000000001

int tests_passed = 0;
int tests_failed = 0;

#define ASSERT(cond, name) \
    if (cond) {printf("PASS: %s\n", name); tests_passed++;} \
    else {printf("FAIL: %s\n", name); tests_failed++;}

void test_create_basic() {
    Matrix *m = matrix_create(3, 3);
    ASSERT(m != NULL, "create: returns non-null");
    
    matrix_free(m);
}

void test_create_zero_rows() {
    Matrix *m = matrix_create(0, 2);
    ASSERT(m == NULL, "create: returns null on 0 rows");
}

void test_create_zero_cols() {
    Matrix *m = matrix_create(3, 0);
    ASSERT(m == NULL, "create: returns null on 0 cols");
}

void test_create_neg_rows() {
    Matrix *m = matrix_create(-1, 2);
    ASSERT(m == NULL, "create: returns null on negative rows");
}

void test_create_neg_cols() {
    Matrix *m = matrix_create(2, -1);
    ASSERT(m == NULL, "create: returns null on negative cols");
}

void test_create_one_by_one() {
    Matrix *m = matrix_create(1, 1);
    ASSERT(m != NULL, "create: returns non-null for 1x1");

    matrix_free(m);
}

// Visual test for checking matrix_print
void test_print_visual() {
    Matrix *m = matrix_create(3, 3);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            matrix_set(m, i, j, (i + j) * 2.5);
        }
    }
    printf("Visual check for matrix_print:\n");
    matrix_print(m);

    matrix_free(m);
}

void test_copy_rows() {
    Matrix *src = matrix_create(2, 2);
    Matrix *copy = matrix_copy(src);
    ASSERT(src->rows == copy->rows, "copy: row count copied");

    matrix_free(src);
    matrix_free(copy);
}

void test_copy_cols() {
    Matrix *src = matrix_create(2, 2);
    Matrix *copy = matrix_copy(src);
    ASSERT(src->cols == copy->cols, "copy: col count copied");

    matrix_free(src);
    matrix_free(copy);
}

void test_copy_null() {
    Matrix *src = NULL;
    Matrix *copy = matrix_copy(src);
    ASSERT(copy == NULL, "copy: returns NULL on bad input");
}

void test_copy_data() {
    int is_same = 1;
    
    Matrix *src = matrix_create(2, 2);
    matrix_set(src, 0, 0, 21.2);
    matrix_set(src, 0, 1, 3.1);
    matrix_set(src, 1, 0, 3.14);
    matrix_set(src, 1, 1, 9);

    Matrix *copy = matrix_copy(src);

    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            if(fabs(matrix_get(src, i, j) - matrix_get(copy, i, j)) > EPSILON) {
                is_same = 0;
            }
        }
    }
    ASSERT(is_same, "copy: data copied successfully");

    matrix_free(src);
    matrix_free(copy);
}

void test_copy_independence() {
    double copy_new_val = 7.11;

    Matrix *src = matrix_create(2, 2);
    Matrix *copy = matrix_copy(src);
    matrix_set(copy, 0, 0, copy_new_val);

    int is_independent = fabs(matrix_get(src, 0, 0) - 0.0) < EPSILON &&
        fabs(matrix_get(copy, 0, 0) - copy_new_val) < EPSILON;

    ASSERT(is_independent, "copy: src and copy independent");
    matrix_free(src);
    matrix_free(copy);
}

void test_set_basic() {
    Matrix *m = matrix_create(2, 2);
    matrix_set(m, 0, 0, 3.14);

    ASSERT(fabs(matrix_get(m, 0, 0) - 3.14) < EPSILON, "set: correctly set value");
    matrix_free(m);
}

void test_get_basic() {
    Matrix *m = matrix_create(2, 2);
    matrix_set(m, 0, 1, 5.4);

    ASSERT(fabs(matrix_get(m, 0, 1) - 5.4) < EPSILON, "get: returns correct value");
    matrix_free(m);
}

void test_get_null() {
    Matrix *m = NULL;
    ASSERT(fabs(matrix_get(m, 0, 0) - 0.0) < EPSILON, "get: returns 0 when passed NULL");
}

void test_get_neg_row() {
    Matrix *m = matrix_create(2, 2);
    ASSERT(fabs(matrix_get(m, -1, 0) - 0.0) < EPSILON, "get: returns 0 when negative row");

    matrix_free(m);
}

void test_get_neg_col() {
    Matrix *m = matrix_create(2, 2);
    ASSERT(fabs(matrix_get(m, 0, -1) - 0.0) < EPSILON, "get: returns 0 when negative col");

    matrix_free(m);
}

void test_get_high_row() {
    Matrix *m = matrix_create(2, 2);
    ASSERT(fabs(matrix_get(m, 2, 0) - 0.0) < EPSILON, "get: returns 0 when row too high");

    matrix_free(m);
}

void test_get_high_col() {
    Matrix *m = matrix_create(2, 2);
    ASSERT(fabs(matrix_get(m, 0, 2) - 0.0) < EPSILON, "get: returns 0 when col too high");

    matrix_free(m);
}

void test_add_null_m1() {
    Matrix *m1 = NULL;
    Matrix *m2 = matrix_create(2, 2);

    Matrix *sum = matrix_add(m1, m2);
    ASSERT(sum == NULL, "add: returns NULL when passed NULL");

    matrix_free(m2);
}

void test_add_null_m2() {
    Matrix *m1 = matrix_create(2, 2);
    Matrix *m2 = NULL;

    Matrix *sum = matrix_add(m1, m2);
    ASSERT(sum == NULL, "add: returns NULL when passed NULL");
    
    matrix_free(m1);
}

void test_add_diff_rows() {
    Matrix *m1 = matrix_create(2, 2);
    Matrix *m2 = matrix_create(3, 2);

    Matrix *sum = matrix_add(m1, m2);
    ASSERT(sum == NULL, "add: returns NULL when rows unequal");

    matrix_free(m1);
    matrix_free(m2);
}

void test_add_diff_cols() {
    Matrix *m1 = matrix_create(2, 2);
    Matrix *m2 = matrix_create(2, 3);

    Matrix *sum = matrix_add(m1, m2);
    ASSERT(sum == NULL, "add: returns NULL when cols unequal");

    matrix_free(m1);
    matrix_free(m2);
}

void test_add_full() {
    Matrix *m1 = matrix_create(2, 2);
    Matrix *m2 = matrix_create(2, 2);

    matrix_set(m1, 0, 0, 1);
    matrix_set(m1, 0, 1, 2);
    matrix_set(m1, 1, 0, 3);
    matrix_set(m1, 1, 1, 4);

    matrix_set(m2, 0, 0, 5);
    matrix_set(m2, 0, 1, 6);
    matrix_set(m2, 1, 0, 7);
    matrix_set(m2, 1, 1, 8);

    Matrix *sum = matrix_add(m1, m2);
    int is_sum= 1;

    for (int i = 0; i < 4; i++) {
        if (fabs(sum->data[i] - (m1->data[i] + m2->data[i])) >= EPSILON) {
            is_sum = 0;
        }
    }

    ASSERT(is_sum, "add: properly adds elements of matrices");

    matrix_free(m1);
    matrix_free(m2);
    matrix_free(sum);
}

void test_subtract_null_m1() {
    Matrix *m1 = NULL;
    Matrix *m2 = matrix_create(2, 2);

    Matrix *diff = matrix_subtract(m1, m2);
    ASSERT(diff == NULL, "subtract: returns NULL when passed NULL");

    matrix_free(m2);
}

void test_subtract_null_m2() {
    Matrix *m1 = matrix_create(2, 2);
    Matrix *m2 = NULL;

    Matrix *diff = matrix_subtract(m1, m2);
    ASSERT(diff == NULL, "subtract: returns NULL when passed NULL");
    
    matrix_free(m1);
}

void test_subtract_diff_rows() {
    Matrix *m1 = matrix_create(2, 2);
    Matrix *m2 = matrix_create(3, 2);

    Matrix *diff = matrix_subtract(m1, m2);
    ASSERT(diff == NULL, "subtract: returns NULL when rows unequal");

    matrix_free(m1);
    matrix_free(m2);
}

void test_subtract_diff_cols() {
    Matrix *m1 = matrix_create(2, 2);
    Matrix *m2 = matrix_create(2, 3);

    Matrix *diff = matrix_subtract(m1, m2);
    ASSERT(diff == NULL, "subtract: returns NULL when cols unequal");

    matrix_free(m1);
    matrix_free(m2);
}

void test_subtract_full() {
    Matrix *m1 = matrix_create(2, 2);
    Matrix *m2 = matrix_create(2, 2);

    matrix_set(m1, 0, 0, 3);
    matrix_set(m1, 0, 1, 9);
    matrix_set(m1, 1, 0, 2);
    matrix_set(m1, 1, 1, 4);

    matrix_set(m2, 0, 0, 5);
    matrix_set(m2, 0, 1, 6);
    matrix_set(m2, 1, 0, -1);
    matrix_set(m2, 1, 1, 3);

    Matrix *diff = matrix_subtract(m1, m2);
    int is_diff = 1;

    for (int i = 0; i < 4; i++) {
        if (fabs(diff->data[i] - (m1->data[i] - m2->data[i])) >= EPSILON) {
            is_diff = 0;
        }
    }

    ASSERT(is_diff, "subtract: properly subtracts elements of matrices");

    matrix_free(m1);
    matrix_free(m2);
    matrix_free(diff);
}

void test_scale_null() {
    Matrix *m = NULL;
    double scalar = 3.00;

    Matrix *scaled_m = matrix_scale(m, scalar);
    ASSERT(scaled_m == NULL, "scale: returns NULL when passed NULL");
}

void test_scale_zero() {
    Matrix *m = matrix_create(2, 2);
    double scalar = 0;

    matrix_set(m, 0, 0, 1);
    matrix_set(m, 0, 1, 2);
    matrix_set(m, 1, 0, 3);
    matrix_set(m, 1, 1, 4);

    Matrix *scaled_m = matrix_scale(m, scalar);
    int is_scaled = 1;

    for (int i = 0; i < (scaled_m->rows * scaled_m->cols); i++) {
        if (fabs(scaled_m->data[i] - (m->data[i] * scalar)) >= EPSILON) {
            is_scaled = 0;
        }
    }

    ASSERT(is_scaled, "scale: scaled matrix properly");
    matrix_free(m);
    matrix_free(scaled_m);
}

void test_scale_full() {
    Matrix *m = matrix_create(2, 2);
    double scalar = 3.14;

    matrix_set(m, 0, 0, 1);
    matrix_set(m, 0, 1, 2);
    matrix_set(m, 1, 0, 3);
    matrix_set(m, 1, 1, 4);

    Matrix *scaled_m = matrix_scale(m, scalar);
    int is_scaled = 1;

    for (int i = 0; i < (scaled_m->rows * scaled_m->cols); i++) {
        if (fabs(scaled_m->data[i] - (m->data[i] * scalar)) >= EPSILON) {
            is_scaled = 0;
        }
    }

    ASSERT(is_scaled, "scale: scaled matrix properly");
    matrix_free(m);
    matrix_free(scaled_m);
}

void test_transpose_null() {
    Matrix *m = NULL;

    Matrix *m_transpose = matrix_transpose(m);
    ASSERT(m_transpose == NULL, "transpose: returns NULL when passed NULL");
}

void test_transpose_visual() {
    Matrix *m = matrix_create(3, 3);

    for (int i = 0; i < m->rows * m->cols; i++) {
        m->data[i] = i + 1;
    }

    Matrix *m_transpose = matrix_transpose(m);
    
    printf("tranpose: Original matrix\n");
    matrix_print(m);

    printf("transpose: Transposed matrix\n");
    matrix_print(m_transpose);

    matrix_free(m);
    matrix_free(m_transpose);
}

void test_transpose_full() {
    Matrix *m = matrix_create(3, 2);

    MAT(m, 0, 0) = 3;
    MAT(m, 0, 1) = 5;
    MAT(m, 1, 0) = 4;
    MAT(m, 1, 1) = 8;
    MAT(m, 2, 0) = 1;
    MAT(m, 2, 1) = 5;

    Matrix *m_transpose = matrix_transpose(m);

    int is_transposed = 1;

    for (int i = 0; i < m_transpose->rows; i++) {
        for (int j = 0; j < m_transpose->cols; j++) {
            if (MAT(m_transpose, i, j) - MAT(m, j, i) > EPSILON) {
                is_transposed = 0;
            }
        }
    }

    ASSERT(is_transposed, "transpose: matrix correctly transposed");
    matrix_free(m);
    matrix_free(m_transpose);
}

void test_multiply_null() {
    Matrix *m1 = NULL;
    Matrix *m2 = NULL;

    Matrix* m_product = matrix_multiply(m1, m2);
    ASSERT(m_product == NULL, "multiply: returns NULL when passed NULL");
}

void test_multiply_dim() {
    Matrix *m1 = matrix_create(3, 4);
    Matrix *m2 = matrix_create(2, 3);

    Matrix *m_product = matrix_multiply(m1, m2);
    ASSERT(m_product == NULL, "multiply: returns NULL with non-matching dimensions");

    matrix_free(m1);
    matrix_free(m2);
}

void test_multiply_full() {
    Matrix *m1 = matrix_create(2, 2);
    Matrix *m2 = matrix_create(2, 1);

    for (int i = 0; i < m1->rows * m1->cols; i++) {
        m1->data[i] = i + 1;
    }

    MAT(m2, 0, 0) = 5;
    MAT(m2, 1, 0) = 2;

    Matrix *m_product = matrix_multiply(m1, m2);

    int dims_correct = m_product->rows == m1->rows && m_product->cols == m2->cols;
    int vals_correct = MAT(m_product, 0, 0) - 9.00 < EPSILON && MAT(m_product, 1, 0) - 23.00 < EPSILON;
    ASSERT(dims_correct && vals_correct, "multiply: correctly multiplied matrices");

    matrix_free(m1);
    matrix_free(m2);
    matrix_free(m_product);
}

void test_identity_dim() {
    Matrix *I = matrix_identity(-1);
    ASSERT(I == NULL, "identity: returns NULL with bad n");
}

void test_identity_full() {
    Matrix *I = matrix_identity(3);

    int is_one = 1;

    for (int i = 0; i < 3; i++) {
        if (MAT(I, i, i) - 1.00 > EPSILON) is_one = 0;
    }

    ASSERT(is_one, "identity: ones on diagonal");
    matrix_free(I);
}

int main() {
    test_create_basic();
    test_create_zero_rows();
    test_create_zero_cols();
    test_create_neg_rows();
    test_create_neg_cols();
    test_create_one_by_one();
    test_copy_rows();
    test_copy_cols();
    test_copy_null();
    test_copy_data();
    test_copy_independence();
    test_set_basic();
    test_get_basic();
    test_get_null();
    test_get_neg_row();
    test_get_neg_col();
    test_get_high_row();
    test_get_high_col();
    test_print_visual();
    test_add_null_m1();
    test_add_null_m2();
    test_add_diff_rows();
    test_add_diff_cols();
    test_add_full();
    test_subtract_null_m1();
    test_subtract_null_m2();
    test_subtract_diff_rows();
    test_subtract_diff_cols();
    test_subtract_full();
    test_scale_null();
    test_scale_zero();
    test_scale_full();
    test_transpose_null();
    test_transpose_visual();
    test_transpose_full();
    test_multiply_null();
    test_multiply_dim();
    test_multiply_full();
    test_identity_dim();
    test_identity_full();

    printf("\n%d passed, %d failed\n", tests_passed, tests_failed);
    return tests_failed > 0 ? 1 : 0;
}