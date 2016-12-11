#include <stdio.h>
#include <pthread.h>
#define MAX_DIM 100
typedef int elements[MAX_DIM][MAX_DIM];
struct _matrix {
  int dim;
  int **M;
};
typedef struct _matrix matrix;

struct _row_n_col {
  matrix *A;
  matrix *B;
  matrix *C;
  pthread_mutex_t mutex;
  int cur; 
};
typedef struct _row_n_col row_n_col;

struct _arg_det {
  matrix *A;
  long int *det;
  pthread_mutex_t mutex;
  int cur;
};
typedef struct _arg_det arg_det;

void dot_product(matrix *A, matrix *B, matrix *C,
		 int i, int j);
void mat_product(matrix *A, matrix *B, matrix **AB);
void* start_mul(void * p);
void m_printer(matrix *A, FILE *f);
matrix * read_matrix_from_file(FILE *F, int DIM);
long int determinant(long A[MAX_DIM][MAX_DIM], int n);
long int D(matrix *A);
void * _det(void *);
int oddp (int n);
