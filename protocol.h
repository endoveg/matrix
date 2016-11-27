#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#define MAXLENGTH 120

struct _msgbuf {
	long mtype;
	char text[MAXLENGTH];
};
typedef struct _msgbuf mbuf;

mbuf * hello;
hello = (mbuf *)malloc(sizeof(mbuf));
hello->mtype = 1;
hello->text = "Hello, world\n";

mbuf * kek;
kek = (mbuf *)malloc(sizeof(mbuf));
kek->mtype = 1;
kek->text = "kek\n";
