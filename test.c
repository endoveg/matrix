#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *FAM = fopen("A","r");
  FILE *FBM = fopen("B","r");
  FILE *FCM = fopen("C","w");
  matrix *A, *B;
  A = read_matrix_from_file(FAM,20);
  B = read_matrix_from_file(FBM,20);
  //m_printer(A);
  //m_printer(B);
  matrix *C;
  mat_product(A, B, &C);
  m_printer(C, FCM); 
  fclose(FCM);
  fclose(FAM);
  fclose(FBM);
  return 0;
}
