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
#define THREAD_NUM 4

void m_printer(matrix *A, FILE *f) {
  int i, j;
  for(i = 0; i < A->dim; i++) {
    for(j=0; j < A->dim; j++) {
      fprintf(f,"%d ", A->M[i][j]);
    }
    fprintf(f,"\n");
  }
}

matrix * init(int dim) {
  int i;
  matrix * A = (matrix *) malloc(sizeof(matrix));
  A->dim = dim;
  A->M = (int **)malloc(sizeof(int*)*dim);
  for (i = 0; i < dim; i++) 
    A->M[i] = (int *)malloc(sizeof(int)*dim);
  return A;
}

void mat_product(matrix *A, matrix *B, matrix **AB) {
  assert(A->dim == B->dim);
  int n = A->dim;
  matrix *C = init(n);
  int pth;
  int j;
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
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
  matrix *p = init(DIM);
  for (i=0; i<DIM; i++) {
    for(j=0; j<DIM; j++) {
      fscanf(F,"%d", &(p->M[i][j]));
    }
  }
  return p;
}

/*long int determinant(long M[MAX_DIM][MAX_DIM], int n) {
  int i, j, minor_no, j_offset;
  long int  det = 0;
  long minor[MAX_DIM][MAX_DIM];  
  if (n == 2) {
    det = M[0][0]*M[1][1] - M[1][0]*M[0][1];
  } else {
    for(minor_no = 0; minor_no < n; minor_no++) {
      minor = (long int **)malloc(sizeof(int *) * (n-1));
      for(i = 0; i < n; i++) 
	minor[i] = (long int *)malloc(sizeof(int) * (n-1));v
      for(i = 0; i < n-1; i++) {
	j_offset = 0;
	for(j = 0; j < n; j++) {
	  if(j == minor_no)
	    continue;
	  else {
	    minor[i][j] = M[i+1][j_offset];
	    j_offset += 1;
	  }
	}
      }
      det += oddp(minor_no + 1 + 1) * M[0][minor_no] * determinant(minor, n-1);
    }
  }
  return(det);
}

int oddp (int n) {
  if (n % 2)
    return 1;
  else
    return -1; 
}

long int D(matrix * A) {
  int j;
  pthread_t thids[THREAD_NUM];
  long int *det;
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  arg_det arg;
  det = (long int *)malloc(sizeof(long int));
  arg.det = det;
  arg.A = A;
  arg.mutex = mutex;
  arg.cur = 0;
  arg.det = 0;
  for(j=0; j < THREAD_NUM; j++)
    pthread_create(thids+j, NULL, &_det, (void*) &arg);
  for (j=0; j < THREAD_NUM; j++)
    pthread_join(thids[j],NULL);
  return *arg.det;
}

void * _det(void * arg) {
  arg_det *p = (arg_det *) arg;
  int n = p->A->dim;
  int q;
  while(1) {
    pthread_mutex_lock(&(p->mutex));
    q = p->cur;
    if (q >= n) {
      pthread_mutex_unlock(&(p->mutex));
      break;
    }
    p->cur += 1;
    *(p->det) += determinant(p->A->M,n);
    pthread_mutex_unlock(&(p->mutex));
  }
  return (void *) NULL;
}

*/
