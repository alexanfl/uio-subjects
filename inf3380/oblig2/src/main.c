#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

/*
 * program: main.c
 *
 * Takes two binary files containing matrices
 * and multiplies them.
 * The result is saved to a new binary file.
 *
 * The process is sped up using Cannon's algorithm
 * in parallel with MPI and OpenMP.
 *
 * Example of compilation and run:
 *
 * mpicc -fopenmp main.c -o main -lm 
 * mpirun -np <num_procs> ./main <infileA.bin> <infileB.bin> <outfileC.bin> 
 *
 * where <number of processes> should be replaced by a square number + 1. 
 * For example 5, 10, 17 ... 
 */

typedef struct{
    /*
     * struct: matrix
     *
     * Has e elements in matrix
     * and s is used for contigous
     * memory allocation of the elements in the array.
     *
     * m rows
     * n columns
     *
     */

    double** e; 
    double* s;
    
    int m;
    int n;
} matrix;

void allocate_matrix(matrix* M, int m, int n){
    /*
     * function: allocate_matrix
     *
     * Allocates memory to a matrix struct.
     * s blocks of consecutive memory stores e elements.
     *
     */
    int i, j;
    M->m = m;
    M->n = n;

    M->s = (double*)malloc(m*n*sizeof(double));
    M->e = (double**)malloc(m*sizeof(double*));

    for (i = 0; i < m; i++) {
        M->e[i] = &(M->s[i*n]);

        for (j = 0; j < n; j++) {
            M->e[i][j] = 0;
        }
    }
}

void deallocate_matrix(matrix* M){
    /*
     * function: deallocate_matrix
     *
     * Frees memory of a matrix struct.
     *
     */

    free(M->s);
    free(M->e);
}

void matrix_multiply(matrix A, matrix B, matrix* C){
    /*
     * function: matrix_multiply
     *
     * Multiplies two matrices A and B into C.
     *
     * OpenMP is used to speed up calculation.
     *
     */

    int i, j, k;

    int m = A.m;
    int l = A.n;
    int n = B.n;

    #pragma omp parallel for private(i,j,k)

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < l; k++) {
                C->e[i][j] += A.e[i][k]*B.e[k][j];
            }
        }
    }
}


void matrix_from_binary(char* filename, matrix* M, int* m, int* n){
    /*
     * function: matrix_from_binary
     *
     * Reads content of binary file to a struct matrix M.
     *
     */

    FILE *fp = fopen(filename, "rb");

    fread(m, sizeof(int), 1, fp);
    fread(n, sizeof(int), 1, fp);

    allocate_matrix(M, *m, *n);

    fread((*M).e[0], sizeof(double), (*m)*(*n), fp);

    fclose(fp);
}

void matrix_to_binary(char* filename, matrix M){
    /*
     * function: matrix_to_binary
     *
     * Writes content of matrix M into a binary file
     *
     */
    FILE *fp = fopen(filename, "wb");

    fwrite(&(M.m), sizeof(int), 1, fp);
    fwrite(&(M.n), sizeof(int), 1, fp);
    fwrite(M.e[0], sizeof(double), M.m*M.n, fp);

    fclose(fp);
}


int main(int argc, char *argv[]){
    int i, j, k, m, l, n;

    int my_rank, num_procs;

    matrix A, B, C;
 
    int iP, jP;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Number of processes must be a square number + 1.    
    int sqP = sqrt(num_procs - 1);

    // The master process delegates work to workers.
    if (my_rank == 0) {
        int worker, mA, nA, mB, nB, mC, nC, lMax;
        
        //Counters
        int iA = 0;
        int jA = 0;
        int iB = 0;
        int jB = 0;

        matrix submatA, submatB, submatC;
    
        if (argc != 4){
            printf("mpirun -np <num_procs> ./main <fileA> <fileB> <fileC>");
            exit(1);
        }
        
        char* infileA = argv[1];
        char* infileB = argv[2];
        char* outfileC = argv[3];

        matrix_from_binary(infileA, &A, &m, &l);
        matrix_from_binary(infileB, &B, &l, &n);

        allocate_matrix(&C, m, n);

        for (worker = 1; worker < num_procs; worker++) {
            // We need to find the coordinates of the current process
            iP = (worker - 1)/sqP;
            jP = (worker - 1)%sqP;

            /* Need to find the sizes of the 
             * submatrices that A and B are split into.
             */

            if (iP < m%sqP) {
                mA = m/sqP + 1;
            }
            else{
                mA = m/sqP;
            }

            if (jP < l%sqP) {
                nA = l/sqP + 1;
            }
            else{
                nA = l/sqP;
            }

            if (0 < l%sqP) {
                lMax = l/sqP + 1;
            }
            else{
                lMax = l/sqP;
            }

            if (iP < l%sqP) {
                mB = l/sqP + 1;
            }
            else{
                mB = l/sqP;
            }
            
            if (jP < n%sqP) {
                nB = n/sqP + 1;
            }
            else{
                nB = n/sqP;
            }

            allocate_matrix(&submatA, mA, lMax);
            allocate_matrix(&submatB, lMax, nB);

            // Create the submatrices
            for (i = 0; i < mA; i++) {
                for (j = 0; j < nA; j++) {
                    submatA.e[i][j] = A.e[i + iA][j + jA];
                }
            }

            for (i = 0; i < mB; i++) {
                for (j = 0; j < nB; j++) {
                    submatB.e[i][j] = B.e[i + iB][j + jB];
                }
            }
    
            // jump to next submatrix elements
            if (worker%sqP == 0) {
                iA += mA;
                iB += mB;

                jA = 0;
                jB = 0;
            }
            else{
                jA += nA;
                jB += nB;
            }


            //Send the submatrices
            MPI_Send(&mA, 1, MPI_INT, worker, 1, MPI_COMM_WORLD);
            MPI_Send(&lMax, 1, MPI_INT, worker, 1, MPI_COMM_WORLD);
            MPI_Send(&nB, 1, MPI_INT, worker, 1, MPI_COMM_WORLD);
            
            MPI_Send(&submatA.e[0][0], mA*lMax, MPI_DOUBLE, worker,
                    1, MPI_COMM_WORLD);
            MPI_Send(&submatB.e[0][0], lMax*nB, MPI_DOUBLE, worker,
                    1, MPI_COMM_WORLD);
           
            deallocate_matrix(&submatA); 
            deallocate_matrix(&submatB); 
        }

        deallocate_matrix(&A);
        deallocate_matrix(&B);

        // Counters for the result matrix
        int iC = 0;
        int jC = 0;

        // The workers sends the results to the master process.
        for (worker = 1; worker < num_procs; worker++) {
            MPI_Recv(&mC, 1, MPI_INT, worker, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&nC, 1, MPI_INT, worker, 1, MPI_COMM_WORLD, &status);

            allocate_matrix(&submatC, mC, nC);

            MPI_Recv(&submatC.e[0][0], mC*nC, MPI_DOUBLE, worker, 
                    1, MPI_COMM_WORLD, &status);

            for (i = 0; i < mC; i++) {
                for (j = 0; j < nC; j++) {
                    C.e[i + iC][j + jC] = submatC.e[i][j];
                }
            }

            jC += nC;
            
            if (worker%sqP == 0) {
                iC += mC;
                jC = 0;
            }

            deallocate_matrix(&submatC);
        }

        matrix_to_binary(outfileC, C);
        deallocate_matrix(&C);
    }

    // Worker processes do work.
    else{
        int i;
        int up, down, right, left;
        int toA, toB, fromA, fromB;

        MPI_Recv(&m, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&l, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&n, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        allocate_matrix(&A, m, l);
        allocate_matrix(&B, l, n);
        allocate_matrix(&C, m, n);

        MPI_Recv(&A.e[0][0], m*l, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&B.e[0][0], l*n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);

        iP = (my_rank - 1)/sqP;
        jP = (my_rank - 1)%sqP;

        // We must align the submatrices for the multiplication algorithm
         
        up = (sqP - jP + iP)%sqP;
        down = (jP + iP)%sqP;
        right = (jP + iP)%sqP;
        left = (sqP + jP - iP)%sqP;

        toA = iP*sqP + left + 1;
        toB = up*sqP + jP + 1;

        fromA = iP*sqP + right + 1;
        fromB = down*sqP +jP + 1;

        // Multiplying and moving submatrices

        if (toA != my_rank) {
            MPI_Sendrecv_replace(&A.e[0][0], m*l, MPI_DOUBLE, toA, 
                    1, fromA, 1, MPI_COMM_WORLD, &status);
        }
        if (toB != my_rank) {
            MPI_Sendrecv_replace(&B.e[0][0], l*n, MPI_DOUBLE, toB, 
                    1, fromB, 1, MPI_COMM_WORLD, &status);
        }

        up = (sqP - 1 + iP)%sqP;
        down = (1 + iP)%sqP;
        right = (jP + 1)%sqP;
        left = (sqP + jP - 1)%sqP;

        toA = iP*sqP + left + 1;
        toB = up*sqP + jP + 1;

        fromA = iP*sqP + right + 1;
        fromB = down*sqP +jP + 1;

        for (i = 0; i < sqP; i++) {
            matrix_multiply(A, B, &C);
            
            MPI_Sendrecv_replace(&A.e[0][0], m*l, MPI_DOUBLE, toA, 
                    1, fromA, 1, MPI_COMM_WORLD, &status);
            MPI_Sendrecv_replace(&B.e[0][0], l*n, MPI_DOUBLE, toB, 
                    1, fromB, 1, MPI_COMM_WORLD, &status);
        }

        // Send result to master process.
        MPI_Send(&m, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&n, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&C.e[0][0], m*n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);

        deallocate_matrix(&A);
        deallocate_matrix(&B);
        deallocate_matrix(&C);
    }

    MPI_Finalize();

    return 0;
}
