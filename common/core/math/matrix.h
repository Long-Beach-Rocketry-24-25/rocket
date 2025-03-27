/**
 * 
 * 
 */

#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LENGTH 500

typedef struct
{
    double* data;
    const size_t rows;
    const size_t cols;
} Matrix;

#define MATRIX(name, rows, cols, ...)              \
    double name##_buf[rows][cols] = {__VA_ARGS__}; \
    Matrix name = {(double*)name##_buf, rows, cols}

#define VAR_MATRIX(name, rows, cols) \
    double name##_buf[rows][cols];   \
    Matrix name = {(double*)name##_buf, rows, cols}

#define MAT_GET(mat, row, col) (mat)->data[((row) * (mat)->cols) + (col)]

Matrix* matrix_copy(Matrix* src, Matrix* dest);

#define MAT_AUGMENT(mat, name)                      \
    VAR_MATRIX(name, (mat)->rows, (mat)->cols * 2); \
    matrix_copy(mat, &name)

Matrix* matrix_multiply(Matrix* A, Matrix* B, Matrix* C);
Matrix* matrix_transpose(Matrix* A, Matrix* AT);
Matrix* matrix_add(Matrix* A, Matrix* B, Matrix* C);
Matrix* matrix_subtract(Matrix* A, Matrix* B, Matrix* C);
Matrix* matrix_inverse(Matrix* A, Matrix* I);
bool matrix_compare(Matrix* A, Matrix* B);
char* matrix_to_string(Matrix* mat);
