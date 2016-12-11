#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE *FAM = fopen("A","r");
  FILE *FCM = fopen("C", "w");
  matrix *A;
  matrix *C = (matrix *)malloc(sizeof(matrix));
  A = read_matrix_from_file(FAM,10);
  C->M = make_minor(A->M, 10, atoi(argv[1]));
  C->dim = 2;
  m_printer(C, FCM);
  fclose(FCM);
  fclose(FAM);
  return 0;
}
