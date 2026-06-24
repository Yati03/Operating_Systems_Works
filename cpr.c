/*------------------------------------------------------------
Fichier: cpr.c

Description: Ce programme contient le code pour la creation
			 d'un processus enfant et y attacher un tuyau.
		 L'enfant envoyera des messages par le tuyau
		 qui seront ensuite envoyes a la sortie standard.

Explication du processus zombie
(point 5 de "A completer" dans le devoir):

	C'est le cas ou un processus a fini sont exécution mais reste dans la liste de procédés.
    Cela arrive dans le cas ou l'on met le procédé en sleep(), on en fait un procédé zombie pendant que le parent tente de lire pour un 'exit status' pendant ce temps.
    Lorsque l'on finit de lire le tuyau, on peut finalement arrêter le processus enfant et récupérer les ressources, détruisant le processus zombie.

-------------------------------------------------------------*/
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h> 

/* Prototype */
void creerEnfantEtLire(int);

/*-------------------------------------------------------------
Function: main
Arguments:
	int ac	- nombre d'arguments de la commande
	char **av - tableau de pointeurs aux arguments de commande
Description:
	Extrait le nombre de processus a creer de la ligne de
	commande. Si une erreur a lieu, le processus termine.
	Appel creerEnfantEtLire pour creer un enfant, et lire
	les donnees de l'enfant.
-------------------------------------------------------------*/

int main(int ac, char **av)
{
	int numeroProcessus;

	if (ac == 2)
	{
		if (sscanf(av[1], "%d", &numeroProcessus) == 1)
		{
			creerEnfantEtLire(numeroProcessus);
		}
		else
			fprintf(stderr, "Ne peut pas traduire argument\n");
	}
	else
		fprintf(stderr, "Arguments pas valide\n");
	return (0);
}

/*-------------------------------------------------------------
Function: creerEnfantEtLire
Arguments:
	int prcNum - le numero de processus
Description:
	Cree l'enfant, en y passant prcNum-1. Utilise prcNum
	comme identificateur de ce processus. Aussi, lit les
	messages du bout de lecture du tuyau et l'envoie a
	la sortie standard (df 1). Lorsqu'aucune donnee peut
	etre lue du tuyau, termine.
-------------------------------------------------------------*/

void creerEnfantEtLire(int prcNum)
{
	int fd[2], pid; /*crée la pipe ainsi que pid qui sert à traquer les valeurs données par fork()*/
	pipe(fd);
	if (prcNum == 1)
	{ /*dernier enfant fin du tuyau*/
		printf("Processus %d commence\n", prcNum);
		sleep(5);
		printf("Processus %d termine\n", prcNum);
		return; /*termine le processus*/
	}
	printf("Processus %d commence\n", prcNum);

    /*maintenant qu'on a commencer on peut fork()*/
	pid = fork(); 

	if (pid == 0) { // enfant
        /*format le char pour execvp afin de recommencer avec une valeur --*/
        char Numb2[11];
		snprintf(Numb2, sizeof(Numb2), "%d", prcNum - 1);
		char* argument_list[] = {"./cpr", Numb2, NULL};
		execvp(argument_list[0], argument_list);
	} else { // parent
        /*ferme write pour pouvoir lire*/
        close(fd[1]);
        char Buffer; 
        while(read(fd[0], &Buffer, sizeof(Buffer))>0){} /*on attends jusqu'a ce que l'enfant est terminer*/
		printf("Processus %d termine\n", prcNum);
        /*ferme la lecture de l'enfant*/
        close(fd[0]);
	}
}
/* S.V.P. completez cette fonction selon les
   instructions du devoirs. */

