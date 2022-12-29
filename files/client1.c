#include "../includes.h"

typedef struct {
	int matiere;
	char pid[10];
}MSG;

int main(int argc, char *argv[]) {


	// Initialisation des variables
	char moyenne[10];
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

		// Création du pipe nommé en lecture
		char path[] = "/tmp/";
		strcat(path, requete.pid);
		mkfifo(path,0666);
		int fd = open(path, O_RDONLY);
		//Lecture
		read(fd, moyenne, 10);

		//Fermeture
		close(fd);

		//Suppression du pipe sur le disque
		char command[30] = "rm ";
		strcat(command, "/tmp/");
		strcat(command, requete.pid);
		printf("%s\n", command);
		system(command);

		//Impression
		float valeur = atof(moyenne);
		printf("Moyenne reçue : %f\n", valeur);


		return EXIT_SUCCESS;
	} else {
		printf("Veuillez spécifier un paramètre numérique entre 1 et 5 inclus.\n");
		return EXIT_FAILURE;
	}
}
