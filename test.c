#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *FAM = fopen("A","r");
  FILE *FBM = fopen("B","r");
  matrix *A, *B;
  A = read_matrix_from_file(FAM,2);
  B = read_matrix_from_file(FBM,2);
  m_printer(A);
  m_printer(B);
  matrix *C;
  mat_product(A, B, &C);
  m_printer(C); 
  return 0;
}
