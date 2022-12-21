#include "../includes.h"

struct MSG {
    int numMatiere;
    char pid[10];
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

    msgrcv(fifo, &msg, sizeof(MSG), 0, 0);
    sleep(2);
    printf("Côté serveur, message reçu\n");
    printf("Numéro de matière : %d\n", msg.numMatiere);
    printf("PID : %s\n", msg.pid);


    mkfifo(msg.pid, 0666);
    char path[20] = "/tmp/";
    strcat(path, msg.pid);
    int fd = open(path, O_WRONLY);

    char moyenne[10];
    sprintf(moyenne, "%f", 10.5);
    write(fd, moyenne, strlen(moyenne)+1);
    close(fd);


    printf("Fin serveur\n");
    return 0;
}
