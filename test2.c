#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *FAM = fopen("A","r");
  matrix *A;
  A = read_matrix_from_file(FAM,2);
  fclose(FAM);
  printf("%ld\n",D(A));
  return 0;
}
