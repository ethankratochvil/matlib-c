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

    printf("\n%d passed, %d failed\n", tests_passed, tests_failed);
    return tests_failed > 0 ? 1 : 0;
}