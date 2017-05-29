#include <stdio.h>
#include <stdlib.h>

void randarray(int n, int *v){
    int i;
    int seed = time(NULL);
    srand(seed);
    for (i = 0; i < n; i++) { 
        v[i] = rand() % 100;
    }
}

void max_of_array(int n, int *v){
    int max = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (max < v[i]) {
            max = v[i];
        }
    }
    printf("The maximum of the array is %d\n", max);
}

void min_of_array(int n, int *v){
    int min = 100;
    int i;
    for (i = 0; i < n; i++) {
        if (min > v[i]) {
            min = v[i];
        }
    }
    printf("The minimum of the array is %d\n", min);
}

int main(){

    int n = 10;
    int *v = (int*)malloc(n*sizeof(int*));
    randarray(n, v);
    max_of_array(n, v);
    min_of_array(n, v);
    return 0;
}
