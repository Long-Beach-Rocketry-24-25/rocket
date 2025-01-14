
#include "matrix.h"


int main(int argc, char* argv[])
{

    double mat[3][3] = {{2, 0, 0}, {0, 2, 0}, {0, 0, 2}};
    double mat2[3][3] = {{0, 0, 2}, {0, 2, 0}, {2, 0, 0}};
    double res[3][3] = {{0}, {0}, {0}};

    printf("A:\n%s", matrix_to_string(mat, 3, 3));
    printf("B:\n%s", matrix_to_string(mat2, 3, 3));

    matrix_add(mat, mat2, res, 3, 3);
    printf("A+B:\n%s", matrix_to_string(res, 3, 3));

    matrix_subtract(mat, mat2, res, 3, 3);
    printf("A-B:\n%s", matrix_to_string(res, 3, 3));

    matrix_multiply(mat, mat2, res, 3, 3, 3);
    printf("A*B\n%s", matrix_to_string(res, 3, 3));

    matrix_transpose(mat2, res, 3, 3);
    printf("T:\n%s", matrix_to_string(res, 3, 3));

    return 0;
}