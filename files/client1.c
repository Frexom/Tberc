#include "../includes.h"

int creerZDC(int size, char *name, int ncle) {

	key_t key = ftok(name, ncle);
	int shmId = shmget(key, size, IPC_CREAT | 0660);

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

typedef struct {
	int matiere;
	char pid[10];
}MSG;

int main(int argc, char *argv[]) {

	// Initialisation des variables
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
		int shmId = creerZDC(sizeof(char[10]), "/etc/passwd", 100);
		printf("ZDC créée : %d\n\n", shmId);

		//Attachement de la ZDC
		char* pZDC = shmat(shmId, (void *) 0, 0);
		if(pZDC  == (char *) -1){
			perror("shmat");
			exit(1);
		};

		//Lecture de la ZDC
		while(pZDC[0] == '*'){
			usleep(200);		//Réessaies dans 0.2 milisecondes
		}
		char* contenu = pZDC;
		printf("CONTENU : %s\n", contenu);
		float moyenne = atof(contenu);

		//Vidage de la ZDC
		memset(pZDC, '\0', strlen(pZDC)+1);
		*pZDC = '*';

		//Détachement de la ZDC
		shmdt(pZDC);

		printf("Moyenne : %f\n", moyenne);


		return EXIT_SUCCESS;
	} else {
		printf("Veuillez spécifier un paramètre numérique entre 1 et 5 inclus.\n");
		return EXIT_FAILURE;
	}
}
