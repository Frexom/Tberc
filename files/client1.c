#include "../includes.h"

int creerZDC(int size, char *name, int ncle) {

	key_t key = ftok(name, ncle);
	int shmId = shmget(key, size, IPC_EXCL);

	if (shmId == -1) {
		printf("La ZDC existe\n");
		shmId = shmget(key, size, IPC_EXCL);
		if(shmId == -1) {
			perror("shmget");
			return -1;
		}
	}
	printf("ZDC : %d\n", shmId);
	return shmId;
}

int detruireZDC(int shmId) {
	int ret = shmctl(shmId, IPC_RMID, NULL);
	if (ret == 0) {
		printf("ZDC d'ID %d détruire\n", shmId);
		return 0;
	} else {
		printf("Erreur pendant la destruction\n");
		return -1;
	}
}


typedef struct {
	int matiere;
	char pid[10];
}MSG;

int main(int argc, char *argv[]) {


	// Initialisation des variables
	char* moyenneStr;
	key_t cle;
	int msgId;
	MSG requete;

	// Vérification du nombre et de la validité des arguments
	if(argc >= 2 && atoi(argv[1]) > 0 && atoi(argv[1]) < 6) {

		//Optention de la clé pour la file de messages
		cle = ftok("/etc/passwd",0);
		if(cle == -1) {
			perror("ftok");
			return EXIT_FAILURE;
		}

		//Création de la file
		msgId = msgget(cle, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		if (msgId == -1) {
			msgId = msgget(cle, IPC_EXCL);
			if(msgId == -1) {
				perror("msgget");
				return EXIT_FAILURE;
			}
			printf("La MSG existait déjà, identificateur : %d\n", msgId);
		}

		// Remplissage du contenu du message
		requete.matiere = atoi(argv[1]);
		sprintf(requete.pid, "%d", getpid());


		// Envoi du message dans la message queue
		if((msgsnd(msgId, &requete, sizeof(MSG) - sizeof(int), 0)) == -1) {
			perror("msgsnd");
			return EXIT_FAILURE;
		}
		else{
			//Impression des logs
			printf("MSG créé avec la clé %x \n", cle);
			printf("Identificateur de la MSG : %d\n", msgId);
			printf("Matière demandée : %d\n", requete.matiere);
			printf("PIDclient : %d\n", getpid());
			printf("Envoi réussi\n\n");
		}

		// Création de la ZDC

		key_t key = ftok("/etc/passwd", 100);
		int shmId = shmget(key, 10, IPC_EXCL);
		printf("ZDC créée : %d\n\n", shmId);

		printf("SHMAT\n");
		char* pZDC =(char*) shmat(shmId, (void *)0, 0);
		printf("ATOF\n");
		moyenneStr = pZDC;
		float moyenne = atof(moyenneStr);
		printf("DETACH\n");
		shmdt(pZDC);

		printf("Moyenne : %f\n", moyenne);
		return EXIT_SUCCESS;
	} else {
		printf("Veuillez spécifier un paramètre numérique entre 1 et 5 inclus.\n");
		return EXIT_FAILURE;
	}
}
