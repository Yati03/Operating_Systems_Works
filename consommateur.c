// Yann Giffaux 0300237500
// CSI3531 Devoir 4-Question 3 Consommateur


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

int main(int argc, char *argv[]) {
    int memoire_id;
    struct memoire *memoire_pointeur;
    long *data_pointeur; 
    memoire_id = shmget(MEM_KEY, sizeof(struct memoire), 0666|IPC_CREAT); 
    if (memoire_id == -1) {
        perror("Mémoire partagée a échoué.");
        return 1;
    }
    memoire_pointeur = shmat(memoire_id, NULL, 0);
    if (memoire_pointeur == (void *) -1) {
        perror("Pointeur de la mémoire partagée a échoué.");
        return 1;
    }
    printf("\nConsommateur attends pour le producteur...\n");
    while (!memoire_pointeur -> end) {} 
    data_pointeur = memoire_pointeur -> data;  
    printf("\nProducteur finit, consommateur commence!\n");
    printf("Nombres Catalan demandées: %ld\n", memoire_pointeur -> catalan);
    printf("\nLes nombres Catalan sont: ");
    for(int n = 1; n <= memoire_pointeur -> catalan; n++) {
        long catalan = *data_pointeur;  
        printf("%ld ", catalan);    
        data_pointeur++;    
    }
    printf("\n");   
    if (shmdt(memoire_pointeur) == -1) {
        perror("shmdt n'a pas réussit à ce détacher.");
        return 1;
    }   
    if (shmctl(memoire_id, IPC_RMID, 0) == -1) {
       perror("shmctl n'a pas pu être éxecuté.");
       return 1;
    }   
    printf("\nConsommateur Status: FINIT\n");
    return 0;
}