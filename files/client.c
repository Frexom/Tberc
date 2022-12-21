#include "../includes.h"

typedef struct {
	int matiere;
	char pid[10];
}MSG;

int main(int argc, char *argv[]) {


	// Arguments
	char moyenne[10];
	key_t cle;
	int msgId;
	MSG requete;

	// Vérif nombre d'arguments
	if(argc == 2) {
		if(atoi(argv[1]) > 0 && atoi(argv[1]) < 6) {

			// Création du message en mémoire

			cle = ftok("/etc/passwd",0);
			if(cle == -1) {
				perror("ftok");
				return EXIT_FAILURE;
			}

			msgId = msgget(cle, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
			if (msgId == -1) {
				msgId = msgget(cle, IPC_EXCL);
				if(msgId == -1) {
					perror("msgget");
					return EXIT_FAILURE;
				}
				printf("La MSG existait déjà, identificateur : %d\n", msgId);
			}

			// Remplissage de la struct

			requete.matiere = atoi(argv[1]);
			sprintf(requete.pid, "%d", getpid());

			// Envoi du message dans la message queue
			if((msgsnd(msgId, &requete, sizeof(MSG) - sizeof(int), 0)) == -1) {
				perror("msgsnd");
				return EXIT_FAILURE;
			}
			else{
				printf("MSG créé avec la clé %x \n", cle);
				printf("Identificateur de la MSG : %d\n", msgId);
				printf("Matière demandée : %d\n", requete.matiere);
				printf("PIDclient : %d\n", getpid());
				printf("Envoi réussi\n\n");

			}

			// Pipe nommé
			char path[] = "/tmp/";
			strcat(path, requete.pid);
			mkfifo(path,0666);
			int fd = open(path, O_RDONLY);
			read(fd, moyenne, 10);
			close(fd);

			float valeur = atof(moyenne);
			printf("Moyenne reçue : %f\n", valeur);

		} else {
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
}
