#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
  FILE *FAM = fopen("A","r");
  FILE *RES = fopen("res", "w");
  matrix *A;
  A = read_matrix_from_file(FAM,12);
  fprintf(RES,"%lli\n", D(A));
  matrix_cleaner(A);
  fclose(FAM);
  fclose(RES);
  return 0;
}


