#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR(message)                          \
  do {                                          \
    perror(message);                            \
    exit(EXIT_FAILURE);                         \
  } while (0)

static size_t readsize (const char * str) {
  size_t size;

  errno = 0;
  size = strtoul(str, NULL, 0);

  if (errno)
    ERROR("readsize");

  return size;
}

int main (int argc, char * argv[]) {
  size_t i, j, k;
  size_t m, n, o;
  int *** cube;

  if (argc < 4) {
    fprintf(stderr, "usage: %s m n l\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  m = readsize(argv[1]);
  n = readsize(argv[2]);
  o = readsize(argv[3]);

  cube = malloc(m * sizeof(int **));

  if (cube == NULL)
    ERROR("malloc");

  cube[0] = malloc(m * n * sizeof(int *));

  if (cube[0] == NULL)
    ERROR("malloc");

  /* first layer of pointers */
  for (i = 0; i < m; i++)
    cube[i] = cube[0] + n*i;

  cube[0][0] = malloc(m * n * o * sizeof(int));

  if (cube[0][0] == NULL)
    ERROR("malloc");

  /* second layer of pointers */
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
      cube[i][j] = cube[0][0] + o*j + o*n*i;

  /* values */
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
      for (k = 0; k < o; k++)
	cube[i][j][k] = 2*i + 3*j + 5*k;

  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
      for (k = 0; k < o; k++)
	printf("%zu, %zu, %zu : %2d\n",
	       i, j, k, cube[i][j][k]);

  free(cube[0][0]);
  free(cube[0]);
  free(cube);

  exit(EXIT_SUCCESS);
}
