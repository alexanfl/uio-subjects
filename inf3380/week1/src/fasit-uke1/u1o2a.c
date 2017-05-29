

// want to find min, max + time; average and std.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[]) {
  
  FILE *data_file  = fopen(argv[1], "r");

  char *buf_line;
  size_t l = 0;

  float min, max, tmp, ex, ex2;
  int n = 1;
  char time[5];   // format: hh:mm
  char tmp_str[6];
  memset(tmp_str, '\0', 6);

  getline(&buf_line, &l, data_file);  // to get something for start
  memcpy(&tmp_str, &buf_line[6], 5);
  tmp = atof(tmp_str);
  min = tmp;
  max = tmp;
  ex = tmp;
  ex2 = tmp*tmp;

  while(getline(&buf_line, &l, data_file)!=-1) {
    n++;
    memcpy(&tmp_str, &buf_line[6], 5);
    tmp = atof(tmp_str);
    printf("tmp: %f \n", tmp);

    if (tmp > max)
      max = tmp;
    if (tmp < min)
      min = tmp;
    ex+=tmp;
    ex2+=tmp*tmp;
  }

  float avr = ex/n;
  float std = sqrt(ex2/n-avr*avr);

  printf("max: %f, min: %f, average: %f, standard deviation: %f \n", \
	 max, min, avr, std);
  free(buf_line);

  return 0;
}
