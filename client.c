#include "protocol.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <pthread.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage: ./client <file with matrix A> <DIM>\n");
    return(-1);
  }
  FILE *FIN;
  if ((FIN = fopen(argv[1], "r")) == NULL) {
     printf("%s\n", strerror(errno));
     return(-1);
  };
  int dim = strtol(argv[2], NULL, 10);
  if (errno == ERANGE) {
    printf("%s\n", strerror(errno));
    return(-1);
  }
  key_t key;
  int msgqid, i, j, proj_id, fd;
  char *ipc_file_name = "sema.txt";
  proj_id = 42;
  fd = open(ipc_file_name, O_CREAT | O_RDWR, 0666);
  size_t msgsz; 
  if (fd == -1) {
    printf("%s\n", strerror(errno));
  }
  key = ftok(ipc_file_name, proj_id);
  if ((msgqid = msgget(key, 0666))
      == -1) {
     printf("%s\n", strerror(errno));
     return(-1);
  }
  struct client_msgbuf * request;
  request = (struct client_msgbuf *)malloc(sizeof(struct client_msgbuf));
  msgsz = sizeof(struct client_msgbuf) - sizeof(long);
  request->pid = getpid();
  request->mtype = 1;
  
  if (msgsnd(msgqid,(void*) request, msgsz, 0666) == -1) {
    printf("%s\n", strerror(errno));
  }
  
  int fin;
  fin = open("PHI", O_WRONLY);
  char  c = '1';
  msgsz = sizeof(struct server_msgbuf) - sizeof(long);
  struct server_msgbuf shm;
  if (msgrcv(msgqid, (void *) &shm, msgsz, getpid(), 0) == -1) {
    printf("%s\n", strerror(errno));
  }
  
  int *DATA;
  int shmid_data = shm.shmid;
  DATA = (int *)shmat(shmid_data, NULL, 0);
  if (DATA == (void *) -1) {
    printf("%s\n", strerror(errno));
  }
  for (i=0; i<dim; i++) {
    for (j=0; j<dim; j++) {
      if (fscanf(FIN, "%d", DATA+j+i*dim) != 1) {
	printf("Wrong matrix dimensions\n");
	return(0);
      }
    }
  }
  write(fin,&c, 1);
  if (msgrcv(msgqid, (void *) &shm, msgsz, getpid(), 0) == -1) {
    printf("%s\n", strerror(errno));
    return(-1);
  }
  printf("%llu\n", shm.det);
  return(0);
}
