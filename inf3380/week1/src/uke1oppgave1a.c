#include <stdio.h>

float calc(int precision){
    float prod = 1;
    int i = 1;
    float sum = 1;
    while(i < precision) {
        prod *= -4;
        sum += 1/prod;
        i++;
    }
    return sum;
}

int main(){
    int precision = 11;
    float sum = calc(precision);
    printf("With precision %d, the sum is %f\n", precision, sum);
}
