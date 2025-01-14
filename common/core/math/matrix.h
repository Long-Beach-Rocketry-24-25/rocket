/**
 * 
 * 
 */

#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_STR_LENGTH      500

void matrix_multiply(double *A, double *B, double *C, int m, int n, int p)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            C[i*p + j] = 0;
            for (int k = 0; k < n; k++) 
            {
                C[i*p + j] += A[i*n + k] * B[k*p + j];
            }
        }
    }
}

void matrix_transpose(double *A, double *AT, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            AT[j*m + i] = A[i*n + j];
        }
    }
}

void matrix_add(double *A, double *B, double *C, int m, int n)
{
    for (int i = 0; i < m*n; i++)
    {
        C[i] = A[i] + B[i];
    }
}

void matrix_subtract(double *A, double *B, double *C, int m, int n)
{
    for (int i = 0; i < m*n; i++)
    {
        C[i] = A[i] - B[i];
    }
}

#ifdef NATIVE
char* matrix_to_string(double* matrix, int rows, int cols)
{

    static char str[MAX_STR_LENGTH];
    char temp[50];
    int pos = 0;

    str[0] = '\0';  // Initialize the string

    for (int i = 0; i < rows; i++)
    {
        strcat(str, "[ ");
        pos += 2;

        for (int j = 0; j < cols; j++)
        {
            snprintf(temp, sizeof(temp), "%.4f ", matrix[i*cols + j]);
            
            // Check if adding this number would exceed the buffer
            if (pos + strlen(temp) >= MAX_STR_LENGTH - 3) // -3 for "]\n" and null terminator
            {
                strcat(str, "...]");
                return str;
            }

            strcat(str, temp);
            pos += strlen(temp);
        }

        strcat(str, "]\n");
        pos += 2;
    }

    return str;
}
#endif