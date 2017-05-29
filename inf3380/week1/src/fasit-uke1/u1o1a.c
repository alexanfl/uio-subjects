
#include <stdio.h>
#include <math.h>

int main(void)
{
  float val = 0, i = 1;
  float eps = 8E-8;
  int max_n = 1000, counter = 0;

  while(fabs(val-0.8)>eps && counter++<max_n) {
    val+=i;
    i/=(-4);
    printf("%f \n", val);
  }

  return 0;
}
