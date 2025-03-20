
#include "kalman.h"

typedef struct Kalman Kalman;

typedef struct
{
    Matrix* A;
    Matrix* H;
    Matrix* Q;
    Matrix* R;
    Matrix* (*transition)(Kalman* k, Matrix* u, Matrix* xp);
    void* priv;
} StateTransition;

struct Kalman
{
    Matrix* x;
    Matrix* P;
    StateTransition t_model;
};

// Matrix* AltitudeKalmanTransition(Kalman* k)
// {
//     k->t_model.transition(k);
// }

bool predict(Kalman* k, Matrix* u)
{
    VAR_MATRIX(xp, k->x->rows, k->x->cols);
    k->t_model.transition(k, u, &xp);
    if (matrix_copy(&xp, k->x) == NULL)
    {
        return false;
    }

    // Calculate P prediction, Pk- = A * Pk-1 * AT + Q
    VAR_MATRIX(A_P, k->P->rows, k->P->cols);
    VAR_MATRIX(AT, k->P->rows, k->P->cols);

    matrix_multiply(k->t_model.A, k->P, &A_P);
    matrix_transpose(k->t_model.A, &AT);
    if (matrix_add(matrix_multiply(&A_P, &AT, k->P), k->t_model.Q, k->P) ==
        NULL)
    {
        return false;
    }

    return true;
}

bool estimate(Kalman* k, Matrix* z)
{
    VAR_MATRIX(gain, k->x->rows, z->rows);

    // Compute Kalman gain
    {
        VAR_MATRIX(HT, k->t_model.H->cols, k->t_model.H->rows);
        VAR_MATRIX(P_HT, k->P->rows, HT.cols);

        matrix_transpose(k->t_model.H, &HT);
        if (matrix_multiply(k->P, &HT, &P_HT) == NULL)
        {
            return false;
        }

        // (H * Pk * HT + R )^-1
        VAR_MATRIX(H_P, k->t_model.H->rows, k->P->cols);
        VAR_MATRIX(H_P_HT_R, k->t_model.R->rows, k->t_model.R->cols);
        VAR_MATRIX(I, H_P_HT_R.cols, H_P_HT_R.rows);

        matrix_multiply(k->t_model.H, k->P, &H_P);
        matrix_multiply(&H_P, &HT, &H_P_HT_R);
        matrix_add(&H_P_HT_R, k->t_model.R, &H_P_HT_R);
        matrix_inverse(&H_P_HT_R, &I);

        // Calculate kalman gain
        if (matrix_multiply(&P_HT, &I, &gain) == NULL)
        {
            return false;
        }
    }

    // Compute estimate
    {
        VAR_MATRIX(H_x, k->t_model.H->rows, k->x->rows);
        VAR_MATRIX(correc, gain.rows, H_x.cols);
        matrix_subtract(z, matrix_multiply(k->t_model.H, k->x, &H_x), &H_x);
        matrix_multiply(&gain, &H_x, &correc);
        matrix_add(k->x, &correc, k->x);
    }

    // Compute error covariance
    {
        VAR_MATRIX(K_H, gain.rows, k->t_model.H->cols);
        VAR_MATRIX(K_H_P, K_H.rows, k->P->cols);
        matrix_multiply(&gain, k->t_model.H, &K_H);
        matrix_multiply(&K_H, k->P, &K_H_P);
        if (matrix_subtract(k->P, k->P, &K_H_P) == NULL)
        {
            return false;
        }
    }

    return true;
}
