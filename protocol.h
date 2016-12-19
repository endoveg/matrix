#include <sys/types.h>
#include <unistd.h>
#define STATUS_OK 0
#define STATUS_ERR -1
#define SRVR_MSG 1

struct server_msgbuf {
long mtype;
int shmid;
long long det;
};

struct client_msgbuf {
long mtype;
pid_t pid;
};

