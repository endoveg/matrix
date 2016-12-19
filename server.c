#include "matrix.h"
#include <stdlib.h>
#include "protocol.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <fcntl.h>
#define DIM 3

matrix *server_matrix;
int msgqid;
key_t key;
int server_init();
void serve(struct client_msgbuf * request);

int main() {
  FILE *FIN;
  if ((FIN = fopen("server_matrix", "r")) == NULL) {
    printf("Where is server's matrix?\n");
    return(0);
  }
  msgqid = server_init();
  server_matrix = read_matrix_from_file(FIN, DIM);
  struct client_msgbuf *buf;
  buf = (struct client_msgbuf *)malloc(sizeof(struct client_msgbuf));
  size_t msgsz = sizeof(struct client_msgbuf) - sizeof(long);
  while(msgrcv(msgqid, (void *) buf, msgsz,1,0) != -1) {
    serve(buf);
  }
  printf("%s\n", strerror(errno));
  return(0);
}

int server_init() {
  int msg_id;
  int proj_id = 42;
  char *ipc_file_name = "sema.txt";
  int fd = open(ipc_file_name, O_CREAT | O_RDWR, 0666);
  if (fd == -1) {
    printf("%s\n", strerror(errno));
  }
  key = ftok(ipc_file_name, proj_id);
  if ((msg_id = msgget(key, IPC_CREAT | IPC_EXCL | 0666))
      == -1) {
     printf("%s\n", strerror(errno));
     return(-1);
  }
  return msg_id;
}

void serve(struct client_msgbuf * request) {
  int i, j;
  int fifo;
  size_t msgsz = sizeof(struct server_msgbuf) - sizeof(long);
  if ((fifo = mkfifo("PHI", 0666)) == -1 && errno != EEXIST) {
    printf("kek1\n");
    printf("%s\n", strerror(errno));
  }
  int *DATA;
  int shmid_data = shmget(key, sizeof(int)*DIM*DIM, IPC_CREAT | 0666);
  if (shmid_data == -1) {
    printf("%s\n", strerror(errno));
  }
  DATA = ( int *)shmat(shmid_data, NULL, 0);
  if (DATA == (void *) -1) {
    printf("kek2\n");
    printf("%s\n", strerror(errno));
  }
  struct server_msgbuf load;
  load.mtype = request->pid;
  load.shmid = shmid_data;
  if (msgsnd(msgqid,(void*) &load, msgsz, 0666) == -1) {
    printf("kek3\n");
    printf("%s\n", strerror(errno));
  }
  int fin;
  fin = open("PHI", O_RDONLY);
  char * c;
  read(fin, &c, 1);
  matrix * client_matrix = init(DIM);
  for (i = 0; i < DIM; i++) {
    for (j=0; j < DIM; j++) {
      client_matrix->M[i][j] = *(DATA + j + i*DIM);
    }
  }
  struct server_msgbuf ans;
  matrix * C;
  mat_product(client_matrix, server_matrix, &C);
  ans.mtype = request->pid;
  ans.det = D(C);
  if (msgsnd(msgqid,(void*) &ans, msgsz, 0666) == -1) {
    printf("kek4\n");
     printf("%s\n", strerror(errno));
  }
}
