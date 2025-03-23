
#include <math.h>
#include "kalman.h"
#include "matrix.h"

int main(int argc, char* argv[])
{
    // MATRIX(mat, 3, 3, {2, 0, 0}, {0, 2, 0}, {0, 0, 2});
    // MATRIX(mat2, 3, 3, {0, 0, 2}, {0, 2, 0}, {2, 0, 0});
    // MATRIX(res, 3, 3, {0}, {0}, {0});

    // MATRIX(test, 3, 3, {2, 0, 0}, {0, 2, 0}, {0, 0, 2});

    // printf("A:\n%s", matrix_to_string(&mat));
    // printf("B:\n%s", matrix_to_string(&mat2));

    // printf("A+B:\n%s", matrix_to_string(matrix_add(&mat, &mat2, &res)));
    // printf("A-B:\n%s", matrix_to_string(matrix_subtract(&mat, &mat2, &res)));
    // printf("A*B\n%s", matrix_to_string(matrix_multiply(&mat, &mat2, &res)));
    // printf("T:\n%s", matrix_to_string(matrix_transpose(&mat, &res)));
    // printf("I:\n%s", matrix_to_string(matrix_inverse(&test, &res)));

    double sd_p = 3;
    double sd_a = 3;
    double t = 0.1;
    MATRIX(A, 2, 2, {1, t}, {0, 1});
    MATRIX(H, 2, 2, {1, 0}, {0, 0});
    // BMP390: +- .5 hPA, BNO055:
    MATRIX(Q, 2, 2, {pow(t, 4) / 4 * sd_a, pow(t, 3) / 2 * sd_a},
           {pow(t, 3) / 2 * sd_a, pow(t, 2) * sd_a});
    MATRIX(R, 2, 2, {sd_p, 0}, {0, sd_a});
    MATRIX(x, 2, 1, {0}, {0});
    MATRIX(P, 2, 2, {0, 0}, {0, 0});

    Kalman k = {&x, &P, {&A, &H, &Q, &R}};

    for (size_t i = 0; i < 100; ++i)
    {
        double v = 1.0 / t;
        predict(&k);
        MATRIX(z, 2, 1, {i}, {v});
        estimate(&k, &z);
        printf("%zu %f\n%s", i, v, matrix_to_string(k.x));
    }

    return 0;
}