#include "../includes.h"

struct MSG {
    int numMatiere;
}MSG;

int main(int argc, char const *argv[]) {

    //Création de la clé
    key_t key = ftok("/etc/passwd", 0);
    if(key == -1){
        printf("Erreur avec la clé\n");
        return EXIT_FAILURE;
    }

    //Obtention de la file de messages
    int fifo = msgget(key, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if(fifo == -1){
        fifo = msgget(key, IPC_EXCL);
        if(fifo == -1){
            printf("Erreur avec la file\n");
            return EXIT_FAILURE;
        }
    }

    struct MSG msg;

    int error = msgrcv(fifo, &msg, sizeof(MSG), -5, 0);
    printf("Erreur : %d", error);
    printf("Numéro de matière : %d\n", msg.numMatiere);

    printf("Fin\n");
    return 0;
}
