
#include "matrix.h"

int main(int argc, char* argv[])
{
    double mat[3][3] = {{2, 0, 0}, {0, 2, 0}, {0, 0, 2}};
    double mat2[3][3] = {{0, 0, 2}, {0, 2, 0}, {2, 0, 0}};
    double res[3][3] = {{0}, {0}, {0}};

    double* mat_p = (double*)mat;
    double* mat2_p = (double*)mat2;
    double* res_p = (double*)res;

    printf("A:\n%s", matrix_to_string(mat_p, 3, 3));
    printf("B:\n%s", matrix_to_string(mat2_p, 3, 3));

    matrix_add(mat_p, mat2_p, res_p, 3, 3);
    printf("A+B:\n%s", matrix_to_string(res_p, 3, 3));

    matrix_subtract(mat_p, mat2_p, res_p, 3, 3);
    printf("A-B:\n%s", matrix_to_string(res_p, 3, 3));

    matrix_multiply(mat_p, mat2_p, res_p, 3, 3, 3);
    printf("A*B\n%s", matrix_to_string(res_p, 3, 3));

    matrix_transpose(mat2_p, res_p, 3, 3);
    printf("T:\n%s", matrix_to_string(res_p, 3, 3));

    return 0;
}