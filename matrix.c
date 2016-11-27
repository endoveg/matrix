#include "matrix.h"
#include <assert.h>
#include <pthread.h>
#include <errno.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#define THREAD_NUM 3

void m_printer(matrix *A, FILE *f) {
	int i, j;
	for(i = 0; i < A->dim; i++) {
		for(j=0; j < A->dim; j++) {
		  fprintf(f,"%d ", A->M[i][j]);
		}
		fprintf(f,"\n");
	}
}
void mat_product(matrix *A, matrix *B, matrix **AB) {
	*AB =  (matrix *) malloc(sizeof(matrix));
	assert(A->dim == B->dim);
	int n = A->dim;
	int i, j;
	int pth;
	int cur;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	matrix * C = *AB;
	C->dim = n;
	pthread_t thids[n];
	row_n_col pack;
	for (i = 0; i < n; i++) {
		cur = 0;
		pack.i = i;
		pack.A = A;
		pack.B = B;
		pack.C = C;
		pack.mutex = mutex;
		pack.cur = &cur;
		for (j=0; j < THREAD_NUM; j++) {
		  pth = pthread_create(thids+j, NULL, &start_mul, (void*) &pack);
		  assert(pth==0);
		}
		for (j=0; j < THREAD_NUM; j++) {
		  pthread_join(thids[j], NULL);
		}
	}
}

void dot_product(matrix *A, matrix *B, matrix *C, int i, int j) {
	assert(A->dim == B->dim);
	int n = A->dim;
	int k;
	int Cij = 0;
	for (k=0; k<n; k++) {
	  Cij += A->M[i][k] * B->M[k][j];
	}
	C->M[i][j] = Cij;
	return;
}

void* start_mul(void *p) {
	row_n_col * P = (row_n_col *) p;
	assert(P->i < P->B->dim);
	pthread_mutex_t mutex = P->mutex;
	int *cur = P->cur;
	int j;
	while (*cur < (P->A->dim)) {
		pthread_mutex_lock(&mutex);
		j = *cur;
		*cur = *cur + 1;
		pthread_mutex_unlock(&mutex);
		dot_product(P->A,P->B,P->C,P->i,j);
	}
	return (void *) NULL;
}

matrix * read_matrix_from_file(FILE *F, int DIM) {
  int i, j;
  matrix *p;
  p = (matrix *)malloc(sizeof(matrix));
  p->dim = DIM;
  for (i=0; i<DIM; i++) {
    for(j=0; j<DIM; j++) {
      fscanf(F,"%d", &(p->M[i][j]));
    }
  }
  return p;
}
