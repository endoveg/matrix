#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

struct _rk {
	int *ar;
	int num;
	int n;
	pthread_mutex_t * mut;
};

typedef struct _rk rk;


void *start_it_up(void * x) {
	int i;
	rk * params;
	params = (rk *) x;
	pthread_mutex_lock(params->mut);
	for (i=0; i < params->n; i++) {
		params->ar[0]++;
	}
	pthread_mutex_unlock(params->mut);
	for (i=0; i < params->n; i++) {
		params->ar[params->num+1]++;
	}
	return NULL;
}

int main() {
	int ar[4];
	pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
	ar[0] = ar[1] = ar[2] = ar[3] = 0;
	pthread_t pth_id1;
	pthread_t pth_id2;
	rk p1, p2, p3;
	p1.n = 100000000;
	p2.n = 100000000;
	p3.n = 100000000;
	p1.num = 0;
	p2.num = 1;
	p3.num = 2;
	p3.mut = p1.mut = p2.mut = &m;
	p3.ar = p1.ar = p2.ar = ar;
	int pth = pthread_create(&pth_id1, NULL, &start_it_up, (void *) &p2);
	if (pth != 0) {
		printf("%s\n", strerror(errno));
		return(1);
	}
	pth = pthread_create(&pth_id2, NULL, &start_it_up, (void *) &p3);
	if (pth != 0) {
		printf("%s\n", strerror(errno));
		return(1);
	}
	start_it_up(&p1);
	if (pthread_join(pth_id, NULL) != 0) {
		printf("%s\n", strerror(errno));
		return(1);
	}
	printf("%d %d %d\n", ar[0], ar[1], ar[2]. ar[3]);
	return(0);
}
