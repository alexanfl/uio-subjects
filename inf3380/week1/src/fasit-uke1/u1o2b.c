

#include <stdio.h>
#include <stdlib.h>

int main(void) {

  // define array with some numbers
  int i, j, n=10, co=0;
  float c = 0.5;
  float *a = malloc(n*n*sizeof(float));
  float *a_new = malloc(n*n*sizeof(float));
  
  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      a[i*n+j] = co*co++;
      printf("%.2f ", a[i*n+j]);
    }
    printf("\n");
  }

  // smooth
  for(i = 1; i < n-1; i++) {
    for(j = 1; j < n-1; j++) {
      a_new[j+i*n] = a[j+i*n]+c*(a[j+(i-1)*n]+a[j-1+i*n]-4*a[j+i*n]+a[j+1+i*n]+a[j+(i+1)*n]);
      printf("%.2f ", a_new[j+i*n]);
    }
    printf("\n");
  }

	
  return 0;

}
