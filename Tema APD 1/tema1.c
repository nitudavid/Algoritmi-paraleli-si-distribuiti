#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "SortedList.h"
#include <math.h>

typedef struct{
    int id;
    int* index;
    int mapperi;
    int reduceri;
    int nrfisieresursa;
    pthread_mutex_t* mutex;
    pthread_barrier_t* barrier;
    int test;
    List*** matrice;

}cbum;

int binarySearch(int start, int end, int x, int power){
    
    int mid;
    unsigned int nr;
    
    if (start > end){
        return 0;
    }
    else{
        mid = (start + end) / 2;
        nr = pow(mid, power);
        if (x == nr){
            return 1;
        }
        else if (nr > x){
            return binarySearch(start, mid - 1, x, power);
        } 
        else if (nr < x){
            return binarySearch(mid + 1, end, x, power);
        }
    }
    return -1;
}

void *f(void *arg) {
  	cbum argument = *(cbum*)arg;
    FILE* in;
    char numein[100];
    char input[100];
    char txt[100];
    int id = argument.id;
    int j = *(argument.index);
    int nrnumere, numar;
    if(argument.id < argument.mapperi){
        while(j <= argument.nrfisieresursa){
            pthread_mutex_lock(argument.mutex);
            j = *(argument.index);
            if(*(argument.index) <= argument.nrfisieresursa){
                (*(argument.index))++;
            }
            pthread_mutex_unlock(argument.mutex);
            
            if(j <= argument.nrfisieresursa){
                sprintf(numein, "test%d/in%d.txt", argument.test ,j);
                in = fopen(numein, "r");
                fscanf(in, "%s\n", input);
                nrnumere = atoi(input);
                for(int i = 1; i <= nrnumere; i++){
                    fscanf(in, "%s\n", txt);
                    numar = atoi(txt);
                    if (numar == 1){
                        for(int x = 0; x < argument.reduceri; x++){
                            insert(argument.matrice[id][x], 1);
                        }
                    }
                    else{
                        for(int z = 2; z <= (argument.reduceri + 1); z++){
                            int h;
                            if(z == 2) h = 46341;
                            if(z == 3) h = 1291;
                            if(z == 4) h = 216;
                            if(z == 5) h = 74;
                            if(binarySearch(2, h, numar, z)){
                                insert(argument.matrice[id][z - 2], numar);
                            }
                        }
                    }
                }
                fclose(in);
            }
        }
    }
    
    pthread_barrier_wait(argument.barrier);
    
    if(argument.id >= argument.mapperi){
        List* concat = (List*)malloc(sizeof(List));
        TSortedList nod;
        int numeredif[1000];
        int count = 0, flag = 0;
        for (int i = 0; i < argument.mapperi; i++){
            nod = argument.matrice[i][argument.id - argument.mapperi]->first;
            while(nod != NULL){
                insert(concat, nod->value);
                nod = nod->next;
            }
        }
        
        nod = concat->first;
        while(nod != NULL){
            flag = 0;
            for(int k = 0; k < count; k++){
                if(numeredif[k] == nod->value){
                    flag = 1;
                }
            }
            if(flag == 0){
                numeredif[count] = nod->value;
                count++;
            }
            nod = nod->next;
        }
        freeTSortedList(concat->first);
        free(concat);
        
        char out[15];
        sprintf(out,"out%d.txt",argument.id - argument.mapperi + 2);
        FILE *final;
        final = fopen(out,"w");
        fprintf(final,"%d", count);
        fclose(final);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    int Mapper = atoi(argv[1]);
    int Reducer = atoi(argv[2]);
    int r, n;
    pthread_t threads[Mapper + Reducer];
    pthread_mutex_t mutex;
    r = pthread_mutex_init(&mutex, NULL);
    pthread_barrier_t barrier;
    r = pthread_barrier_init(&barrier, NULL, Mapper + Reducer);
    cbum cbums[Mapper + Reducer];
    long id;
  	void *status;
    char nrfisiere[100];
    int numartest;
    int *index = malloc(sizeof(int));
    *index = 1;
    List*** matrice = (List***)malloc(sizeof(List**) * Mapper); 

    for (int i = 0; i < Mapper; i++){
        matrice[i] = (List**)malloc(sizeof(List*) * Reducer); 
        for(int j = 0; j < Reducer; j++){
            matrice[i][j] = (List*)malloc(sizeof(List));
            matrice[i][j]->first = NULL;
            matrice[i][j]->last = NULL;
        }
    }
    
    FILE *file;
    file = fopen(argv[3], "r");
    fscanf(file, "%s\n", nrfisiere);
    n = atoi(nrfisiere);
    
    int c = 0, l;
    while( argv[3][c] ){
        if(argv[3][c] >= 48 && argv[3][c] <= 57){
            l = argv[3][c] - 48;
            break;
        }
        c++;
    }
    numartest = l;

    for (int i = 0; i < Mapper + Reducer; i++){
        cbums[i].id = i;
        cbums[i].index = index;
        cbums[i].mapperi = Mapper;
        cbums[i].reduceri = Reducer;
        cbums[i].nrfisieresursa = n;
        cbums[i].mutex = &mutex;
        cbums[i].test = numartest;
        cbums[i].matrice = matrice;
        cbums[i].barrier = &barrier;
    }

  	for (id = 0; id < Mapper + Reducer; id++) {
		r = pthread_create(&threads[id], NULL, f, &cbums[id]);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

  	for (id = 0; id < Mapper + Reducer; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

    fclose(file);
    r = pthread_mutex_destroy(&mutex);
    r = pthread_barrier_destroy(&barrier);

    for (int i = 0; i < Mapper; i++){ 
        for(int j = 0; j < Reducer; j++){
            freeTSortedList(matrice[i][j]->first);
        }
        free(matrice[i]);
    }
    free(matrice);
    free(index);
    pthread_exit(NULL);
    return 0;
}