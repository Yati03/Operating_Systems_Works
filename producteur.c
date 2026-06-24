

// gcc producer.c -o ./producer
// ./producer 5
// (5 == nombres Catalan)

#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MEM_KEY 0x1111
#define DATA_TAILLE 1024 

struct memoire {
    long catalan;
    bool end;
    long data[DATA_TAILLE];
};

void store_data(long *data_pointer, long numb);

long factoriel(long n);

int main(int argc, char *argv[]) {
    long catalan_num; 
    if (argc == 2) {
        if(sscanf(argv[1], "%ld", &catalan_num) == 1) {
            printf("\nNombres Catalan demandées: %ld\n", catalan_num);
        }
        else {
            fprintf(stderr, "Écris un Integer!.\n");
        }
    }
    else {
        printf("\nPas de valeur écrite. Nombre Catalan est mis à 5.\n");
        catalan_num = 5;
    }
    int memoire_id;
    struct memoire *memoire_pointeur;
    long *data_pointeur;
    memoire_id = shmget(MEM_KEY, sizeof(struct memoire), 0666|IPC_CREAT);   
    if (memoire_id == -1) {
        perror("Mémoire partagée a échoué.");
        return 1;
    }
    memoire_pointeur = shmat(memoire_id, NULL, 0);
    if (memoire_pointeur == (void*) -1) {
        perror("Pointeur de la mémoire partagée a échoué.");
        return 1;
    }
    memoire_pointeur -> catalan = catalan_num;
    data_pointeur = memoire_pointeur->data;
    printf("\nÉcris la data dans la mémoire partagée.\n");
    store_data(data_pointeur, catalan_num);  
    memoire_pointeur -> end = true;
    if (shmdt(memoire_pointeur) == -1) {
        perror("shmdt n'a pas réussit à ce détacher.");
        return 1;
    }
    printf("\nProducteur Status: FINIT\n");
    return 0;
}


void store_data(long *data_pointeur, long numb) {
    long temp = 0;
    for(int n = 1; n <= numb; n++) {
        temp = factoriel(2*n)/(factoriel(n+1)*factoriel(n));
        *data_pointeur = (long)temp;
        data_pointeur++;

        printf("Memoire mise à jour...\n");
        sleep(3); 
    }
    printf("\nData enregistrée\n");
}


long factoriel(long n) {
    long result = 1;
    for(long i = 1; i <= n; i++) {
        result *= i;  
    }
    return result;
}