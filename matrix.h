#include <stdio.h>
#include <pthread.h>
#define MAX_DIM 100
typedef int elements[MAX_DIM][MAX_DIM];
struct _matrix {
  int dim;
  long int M[MAX_DIM][MAX_DIM];
};
typedef struct _matrix matrix;

struct _row_n_col {
	matrix *A;
	matrix *B;
	matrix *C;
	pthread_mutex_t mutex;
	int cur; //current column
};
typedef struct _row_n_col row_n_col;

void dot_product(matrix *A, matrix *B, matrix *C, int i, int j);
void mat_product(matrix *A, matrix *B, matrix **AB);
void* start_mul(void * p);
void m_printer(matrix *A, FILE *f);
matrix * read_matrix_from_file(FILE *F, int DIM);
