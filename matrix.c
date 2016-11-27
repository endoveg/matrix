#include "matrix.h"
#include <assert.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
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
		  fprintf(f,"%ld ", A->M[i][j]);
		}
		fprintf(f,"\n");
	}
}
void mat_product(matrix *A, matrix *B, matrix **AB) {
	matrix *C =  (matrix *) malloc(sizeof(matrix));
	assert(A->dim == B->dim);
	int n = A->dim;
	int pth;
	int j;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	C->dim = n;
	pthread_t thids[THREAD_NUM];
	row_n_col pack;
	pack.A = A;
	pack.B = B;
	pack.C = C;
	pack.mutex = mutex;
	pack.cur = 0;
	for (j=0; j < THREAD_NUM; j++) {
	  pth = pthread_create(thids+j, NULL, &start_mul, (void*) &pack);
	  assert(pth==0);
	}
	for (j=0; j < THREAD_NUM; j++) {
		pthread_join(thids[j],NULL);
	}
	*AB = C;
	pthread_mutex_destroy(&mutex);
}

void dot_product(matrix *A, matrix *B, matrix *C, int i, int j) {
	assert(A->dim == B->dim);
	assert(i < A->dim);
	assert(j < A->dim);
	int n, k, Cij;
	n = A->dim;
	Cij = 0;
	for (k=0; k<n; k++) {
	  Cij += A->M[i][k] * B->M[k][j];
	}
	C->M[i][j] = Cij;
	return;
}

void* start_mul(void *p) {
	row_n_col *P = (row_n_col *) p;
	int i, j, q, n;
	assert(P->A->dim == P->B->dim);
	n = P->A->dim;
	while(1) {
		pthread_mutex_lock(&(P->mutex));
		q = P->cur;
		if (q >= n*n) {
			pthread_mutex_unlock(&(P->mutex));
			break;
		}
		P->cur += 1;
		pthread_mutex_unlock(&(P->mutex));
		i = q / n;
		j = q % n;
		dot_product(P->A, P->B, P->C, i, j);
	}
	return  NULL;
}
matrix * read_matrix_from_file(FILE *F, int DIM) {
  int i, j;
  matrix *p;
  p = (matrix *)malloc(sizeof(matrix));
  p->dim = DIM;
  for (i=0; i<DIM; i++) {
    for(j=0; j<DIM; j++) {
      fscanf(F,"%ld", &(p->M[i][j]));
    }
  }
  return p;
}
