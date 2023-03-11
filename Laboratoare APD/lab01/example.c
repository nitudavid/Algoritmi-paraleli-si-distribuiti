#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// #define NUM_THREADS 128

void *f(void *arg) {
  	long id = (long)arg;

	for (int i = 0; i < 100; i++) {
		printf("Hello World din thread-ul %ld, a %d oara!\n", id, i);
	}

  	// printf("Hello World din thread-ul %ld!\n", id);
  	pthread_exit(NULL);
}

void *g(void *arg) {
	long id = (long)arg;

	for (int i = 0; i < 100; i++) {
		printf("Da nebunelor din thread-ul %ld, a %d oara!\n", id, i);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	long cores = 2; //sysconf(_SC_NPROCESSORS_CONF);
	pthread_t threads[cores];
	void *funcs[2];
  	int r;
  	long id;
  	void *status;

	funcs[0] = f;
	funcs[1] = g;

  	for (id = 0; id < cores; id++) {
		r = pthread_create(&threads[id], NULL, funcs[id], (void *)id);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

  	for (id = 0; id < cores; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

  	pthread_exit(NULL);
}
