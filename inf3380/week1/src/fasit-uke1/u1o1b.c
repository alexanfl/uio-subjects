
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Give one argument, n = size of array \n"); 
    return -1;
  }

  int n = atoi(argv[1]);   // cl argument
  int *a = malloc(n*sizeof(int));
  int i;

  for(i = 0; i < n; i++)
    a[i] = rand()%(n*n);

  int max = a[0];
  int min = a[0];
  
  for(i = 1; i < n; i++) {
    if (a[i]>max)
      max = a[i];
    if (a[i]<min)
      min = a[i];
  }
  printf("max: %d, min: %d \n", max, min);

  return 0;
}
