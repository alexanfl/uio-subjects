
#include <stdio.h>

void swap(int *a, int *b)
{
  int t=*a; *a=*b; *b=t;
}

void sort(int arr[], int perm[], int beg, int end)
{
  if (end > beg + 1) {
    int piv = arr[perm[beg]], l = beg + 1, r = end;
    while (l < r) {
      if (arr[perm[l]] <= piv)
	l++;
      else
	swap(&perm[l], &perm[--r]);
    }
    swap(&perm[--l], &perm[beg]);
    sort(arr, perm, beg, l);
    sort(arr, perm, r, end);
  }
}

int main(void) {
  int a[11] = {30, 121, 122, 11, 43, 9, 21, 123, 2, 99};
  int p[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  sort(a, p, 0, 10);
  int i;
  for(i = 0; i < 10; i++)
    printf("%d ", a[p[i]]);

  return 0;
}
