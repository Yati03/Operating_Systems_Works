

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

// initialisation
#define N_CHAISES 4
#define N_RUNS 12

// fils
pthread_t ta_thread;
pthread_t *etudiants_thread;
pthread_mutex_t chaise_mutex;

// sémaphores
sem_t ta;
sem_t étudiants;  
sem_t chaises[N_CHAISES];
sem_t count_runs; 
sem_t inc_runs; 

// variables
int chaise_prises = 0;
int chaise_next = 0;
int run_counter = 0; 

int delay() {
    return (rand() % 5) + 1;
}


void if_end() {
    if(run_counter == N_RUNS) {
        printf("\n***** EXIT PROGRAMME *****\n");
        exit(0);
    }
}


void *taFunc() {	

	for (;;) {

        sem_wait(&ta);

        printf("\n- TA SE RÉVEILLE -\n");

        for (;;) {
            pthread_mutex_lock(&chaise_mutex);

            if(chaise_prises == 0) {
                printf("\nTA aide l'étudiant.\n");
                sleep(delay());

                sem_post(&étudiants);
                sem_wait(&count_runs);
                run_counter++;

                printf("\n- TA EXÉCUTE LE COMPTE: %d -\n",run_counter);

                if_end();
                sem_post(&inc_runs);

                pthread_mutex_unlock(&chaise_mutex);

                break;
            }
            else {

                sem_post(&chaises[chaise_next]);

                if (chaise_prises > -1) {
                    chaise_prises -= 1;
                }
                else {
                    chaise_prises = -1;
                }

                printf("\nMaintenant, il y a %d chaise(s) disponible.\n", N_CHAISES - chaise_prises);

                chaise_next = (chaise_next + 1) % N_CHAISES;
                pthread_mutex_unlock(&chaise_mutex);

                printf("\nThe TA is currently helping a student.\n");

                sleep(delay());

                sem_post(&étudiants);

                pthread_mutex_lock(&chaise_mutex);

                sem_wait(&count_runs);

                run_counter++;
                printf("\n- TA FAIT LE COMPTE: %d -\n",run_counter);

                if_end();
                
                sem_post(&inc_runs);

                pthread_mutex_unlock(&chaise_mutex);   
            } 
        }
        printf("\n- TA FAIT UNE SIESTE -\n");   
	}
}


void *étudiantFunc(void *s) {
	for (;;) {
        printf("\n>> Étudiant %ld travaille.\n", (long) s);

        sleep(delay());

        pthread_mutex_lock(&chaise_mutex);

        int chaises_temp =  chaise_prises;

        pthread_mutex_unlock(&chaise_mutex);

        if(chaises_temp < N_CHAISES) {
            //printf("\nÉtudiant %ld veut de l'aide.\n", (long) s);

            pthread_mutex_lock(&chaise_mutex);
            int chaise_current =  (chaise_next + chaise_prises) % N_CHAISES; 

            printf("\n>> Étudiant %ld veut de l'aide.\n", (long) s);

            if (chaise_prises < N_CHAISES && chaise_prises >= -1) {
                chaise_prises += 1;

            } 
            else {
                chaise_prises = N_CHAISES;
            }

            printf("\nIl y a %d chaise(s) qui reste.\n", N_CHAISES - chaise_prises);

            pthread_mutex_unlock(&chaise_mutex);

            if(chaises_temp > 0) {

                printf("\nÉtudiant %ld attends le TA.\n", (long) s);

                sem_wait(&chaises[chaise_current]);

                sem_wait(&étudiants);

                printf("\nÉtudiant %ld a finit avec le TA et retourne travailler.\n",(long) s);

                sem_post(&count_runs);

                sem_wait(&inc_runs);
            }
            else {
                sem_post(&ta);

                sem_wait(&étudiants);

                printf("\nÉtudiant %ld a finit avec le TA et retourne travailler.\n",(long) s);

                sem_post(&count_runs);

                sem_wait(&inc_runs);
            } 
        }
        else {
            printf("\nÉtudiant %ld veut que le TA l'aide.\n", (long) s);

            printf("\nTA est occupé. Étudiant %ld retourne travailler.\n", (long) s);
        }   
	}
}


int main(int ac, char **av) {
    int num_étudiants; 

    // arguments
    if(ac == 2) {
        if(sscanf(av[1],"%d",&num_étudiants) == 1) {

            printf("\nNombre d'étudiants : %d\n",num_étudiants);
            }
        else {

            fprintf(stderr,"Entre un nombre integer!\n");
            }
    }
    else {
        num_étudiants = 5;
        printf("\nLe nombre d'étudiants par défaut est: %d.\n",num_étudiants);
    }

    printf("\n* Le programme finit lorque le TA exécute %d fois. *\n", N_RUNS);

    // sémaphores
    sem_init(&ta, 0, 0);
    sem_init(&étudiants, 0, 0);
    sem_init(&count_runs, 0, 0);
	sem_init(&inc_runs, 0, 0);

	for (int i = 0; i < N_CHAISES; i++) {
        sem_init(&chaises[i], 0, 0);
    }		
		
    // mutex
    if (pthread_mutex_init(&chaise_mutex, NULL) != 0) { 
        printf("\nMutex échoue!\n"); 
        return 1;
    } 

    srand(time(NULL));

    printf("\n- TA DORS -\n");

    // fils
    pthread_create(&ta_thread, NULL, taFunc, NULL);

    etudiants_thread = (pthread_t*) malloc(num_étudiants * sizeof(pthread_t));

    for (long i = 0; i < num_étudiants; i++) {
        pthread_create(&etudiants_thread[i], NULL, étudiantFunc, (void*) i);
    }

    // fil rejoins
    pthread_join(ta_thread,NULL);

    for (int i = 0; i < num_étudiants; i++) {
        
        pthread_join(etudiants_thread[i], NULL);
    }
    
	return 0;
}