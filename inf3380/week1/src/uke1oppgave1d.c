#include <stdlib.h>
#include <stdio.h>

void array3D(double*** A, int n){
}

int main(){
    int n = 3;
    double*** A = malloc(n*sizeof(double**));
    
    double* A_storage1 = malloc(n*n*n*sizeof(double));
    double** A_storage2 = malloc(n*n*sizeof(double*));

    int i,j,k;
    for (i=0; i < n; i++){
        A[i] = &(A_storage2[i*n]);
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            A[i][j] = &(A_storage1[j*n + i*n*n]);
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                A[i][j][k] = 2*i + 3*j + 5*k;
            }
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
	printf("%d, %d, %d : %2.0f\n",
	       i, j, k, A[i][j][k]);
            }
        }
    }
    
    free(A[0][0]);
    free(A[0]);
    free(A);

    return 0;
}
