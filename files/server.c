#include "../includes.h"

//Structure du message dans la file de messages
struct MSG {
    int numMatiere;
    char pid[10];
}MSG;

//Structure représentant une matière
typedef struct Matiere {
    int id;
    char nom[20];
    float moyenne;
}Matiere;

//Constructeur d'une Matière
Matiere creerMatiere(int id, char* nom, float moyenne){
        Matiere m;
        m.id = id;
        strcpy(m.nom, nom);
        m.moyenne = moyenne;
        return m;
}


int main(int argc, char const *argv[]) {
    printf("Démarrage du serveur\n");

    //Création des matières
    Matiere moyennes[5];
    moyennes[0] = creerMatiere(1, "Anglais", 12.5);
    moyennes[1] = creerMatiere(2, "Mathématiques", 10.8);
    moyennes[2] = creerMatiere(3, "Informatique", 9.3);
    moyennes[3] = creerMatiere(4, "CultureG", 12.6);
    moyennes[4] = creerMatiere(5, "Physique", 11.8);

    //Pour rester tout le temps actif
    while(1){

        //Création de la clé
        key_t key = ftok("/etc/passwd", 0);
        if(key == -1){
            printf("Erreur avec la clé\n");
            return EXIT_FAILURE;
        }


        //Obtention de la file de messages
        int mesQueue = msgget(key, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
        if(mesQueue == -1){
            mesQueue = msgget(key, IPC_EXCL);
            if(mesQueue == -1){
                printf("Erreur avec la file\n");
                return EXIT_FAILURE;
            }
        }


        struct MSG msg;

        //Obtention du message
        msgrcv(mesQueue, &msg, sizeof(MSG), 0, 0);


        //Impression des logs
        printf("Côté serveur, message reçu\n");
        printf("Numéro de matière : %d\n", msg.numMatiere);
        printf("PID : %s\n", msg.pid);
        printf("Moyenne envoyée : %f\n\n", moyennes[msg.numMatiere-1].moyenne);


        //Création du pipe nommé
        char path[20] = "/tmp/";
        strcat(path, msg.pid);
        mkfifo(path, 0666);
        int fd = open(path, O_WRONLY);

        //Écriture dans le pipe
        char moyenne[10];
        sprintf(moyenne, "%f", moyennes[msg.numMatiere-1].moyenne);
        write(fd, moyenne, strlen(moyenne)+1);
        close(fd);
    }


    printf("Fin serveur\n");
    return 0;
}
