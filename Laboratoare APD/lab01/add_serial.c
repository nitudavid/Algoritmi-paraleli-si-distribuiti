#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
    schelet pentru exercitiul 5
*/

int* arr;
int array_size;
long cores;

void *does_smth(void *arg) {
    long id = *(long *) arg;
    int start = id * (double)array_size / cores;
    int end;

    if ((id + 1) * (double)array_size / cores < array_size) {
        end = (id + 1) * (double)array_size / cores;
    } else {
        end = array_size;
    }

    for (int i = start; i < end; i++) {
        arr[i] += 100;
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    cores = sysconf(_SC_NPROCESSORS_CONF);
    pthread_t threads[cores];
    void *status;
    int r;
    long values[cores];

    array_size = atoi(argv[1]);

    arr = malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

    // for (int i = 0; i < array_size; i++) {
    //     printf("%d", arr[i]);
    //     if (i != array_size - 1) {
    //         printf(" ");
    //     } else {
    //         printf("\n");
    //     }
    // }

    // TODO: aceasta operatie va fi paralelizata
  	// for (int i = 0; i < array_size; i++) {
    //     arr[i] += 100;
    // }

    for (int id = 0; id < cores; id++) {
        values[id] = id;
        r = pthread_create(&threads[id], NULL, does_smth, &values[id]);

        if (r) {
	  		printf("Eroare la crearea thread-ului %d\n", id);
	  		exit(-1);
		}
    }

    for (int id = 0; id < cores; id++) {
        r = pthread_join(threads[id], &status);

        if (r) {
	  		printf("Eroare la asteptarea thread-ului %d\n", id);
	  		exit(-1);
		}
    }

    // for (int i = 0; i < array_size; i++) {
    //     printf("%d", arr[i]);
    //     if (i != array_size - 1) {
    //         printf(" ");
    //     } else {
    //         printf("\n");
    //     }
    // }

  	pthread_exit(NULL);
}
