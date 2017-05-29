#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void retrieveFromFile(char** t, char** T)
{
    FILE *ifp;
    char *mode = "r";
    char filename[25]  = "../res/temperature.txt";

    ifp = fopen(filename, mode);
    
    char x[20];
    char y[20];
    
    int n = 13;

    int i = 0;

    while (fscanf(ifp, "%s\t%s]", x, y) !=EOF){
        strcpy(t[i], x);
        strcpy(T[i], y);
        printf("%s %s\n", t[i], T[i]);
        i++;
    }

    fclose(ifp);

}

double findAverage(char** T, int n){
    double average = 0;
    double z = 0;

    int i;

    for (i = 0; i < n; i++) {
        sscanf(T[i], "%lf", &z);
        average += z;
    }

    average /= n;

    return average;
}

int main(int argc, const char *argv[]){
    int n = 13;
    int m = 20;

    char** t = malloc(n*sizeof(char*));
    char** T = malloc(n*sizeof(char*));

    int i;
    for (i = 0; i < n; i++) {
        t[i] = malloc(m*sizeof(char));
        T[i] = malloc(m*sizeof(char));
    }

    retrieveFromFile(t, T);
    printf("%lf\n", findAverage(T, n));

    free(T[0]);
    free(T);
    return 0;
}
