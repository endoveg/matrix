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

long long int determinant(int** M, int n) {
  int minor_no;
  long long int  det = 0;
  int **minor;  
  if (n == 2) {
    det = M[0][0]*M[1][1] - M[1][0]*M[0][1];
  }else if (n == 1) {
    det = M[0][0];
  }else {
    for(minor_no = 0; minor_no < n; minor_no++) {
      minor = make_minor(M, n, minor_no);
      det += oddp(minor_no) * M[0][minor_no] *
	determinant(minor, n-1);
      free_minor(minor, n-1);
    }
  }
  return(det);
}

int oddp (int n) {
  if (n % 2)
    return -1;
  else
    return 1; 
}

long long int D(matrix * A) {
  int j;
  pthread_t thids[THREAD_NUM];
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  arg_det arg;
  long long int det = 0;
  arg.D = (long long int *)malloc(sizeof(long long int)*A->dim);
  arg.A = A;
  arg.mutex = mutex;
  arg.cur = 0;
  for(j=0; j < THREAD_NUM; j++)
    pthread_create(thids+j, NULL, &_det, (void*) &arg);
  for (j=0; j < THREAD_NUM; j++)
    pthread_join(thids[j],NULL);
  for (j = 0; j < A->dim; j++)
    det += arg.D[j];
  free(arg.D);
  return det;
}

void * _det(void * arg) {
  arg_det *p;
  int n, q;
  int **minor;
  p = (arg_det *) arg;
  n = p->A->dim;
  while(1) {
    pthread_mutex_lock(&(p->mutex));
    q = p->cur;
    if (q >= n) {
      pthread_mutex_unlock(&(p->mutex));
      break;
    }
    p->cur += 1;
    pthread_mutex_unlock(&(p->mutex));
    minor = make_minor(p->A->M, n, q);
    p->D[q] =  oddp(q) * determinant(minor,n-1) * p->A->M[0][q];
    free_minor(minor, n-1);
  }
  return (void *) NULL;
}

int **make_minor(int **M, int n, int minor_no) {
  int i, j, j_offset;
  int ** minor;
  minor = (int **)malloc((n-1)*sizeof(int *));
  for(i = 0; i < n-1; i++) 
    minor[i] = (int *)malloc((n-1)*sizeof(int));
  for(i = 0; i < n-1; i++) {
    j_offset = 0;
    for(j = 0; j < n; j++) {
      if(j == minor_no)
	continue;
      else {
	minor[i][j_offset] = M[i+1][j];
	j_offset += 1;
      }
    }
  }
  return minor;
}

void free_minor(int **minor, int dim) {
  int i;
  for (i=0; i < dim; i++) 
    free(minor[i]);
  free(minor);
}

void matrix_cleaner(matrix *A) {
  int n = A->dim;
  int i;
  for (i=0; i<n; i++)
    free(A->M[i]);
  free(A->M);
  free(A);
}

