
#include "matrix.h"

int main(int argc, char* argv[])
{
    MATRIX(mat, 3, 3, {2, 0, 0}, {0, 2, 0}, {0, 0, 2});
    MATRIX(mat2, 3, 3, {0, 0, 2}, {0, 2, 0}, {2, 0, 0});
    MATRIX(res, 3, 3, {0}, {0}, {0});

    MATRIX(test, 3, 3, {2, 0, 0}, {0, 2, 0}, {0, 0, 2});

    printf("A:\n%s", matrix_to_string(&mat));
    printf("B:\n%s", matrix_to_string(&mat2));

    printf("A+B:\n%s", matrix_to_string(matrix_add(&mat, &mat2, &res)));
    printf("A-B:\n%s", matrix_to_string(matrix_subtract(&mat, &mat2, &res)));
    printf("A*B\n%s", matrix_to_string(matrix_multiply(&mat, &mat2, &res)));
    printf("T:\n%s", matrix_to_string(matrix_transpose(&mat, &res)));
    printf("I:\n%s", matrix_to_string(matrix_inverse(&test, &res)));

    return 0;
}