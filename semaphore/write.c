#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};



int main(void)
{
    key_t key;
    int semid, shmid;
    union semun arg;
    int k;
    char *data;
    char newData[SHM_SIZE];
    struct sembuf sb;
    if ((key = ftok("sem.c", 'J')) == -1) {
        perror("ftok");
        exit(1);
    }
    if ((semid = semget(key, 1, 0666)) == -1) {
        perror("semget");
        exit(1);
    }

    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) { 
        perror("shmget");
        exit(1);
    }
    
    /* grab the semaphore set created by seminit.c: */
    
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = 0;
    
    if (semop(semid, &sb, 1) == -1) {
        printf("couldnt lock");
        perror("semop");
        exit(1);
    }

    data = shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }
    printf("locked\n");
    printf("Data stored in shared memory segment: '%s'\n", data);
    printf("New data to be stored in segment: ");
    scanf("%s", newData);
    strncpy(data, newData, SHM_SIZE);
    getchar();
    sb.sem_num = 0;
    sb.sem_op = 1;   /* <-- Comment 3 */
    sb.sem_flg = 0;
    if (semop(semid, &sb, 1) == -1) {
        printf("couldnt lock");
        perror("semop");
        exit(1);
    }
    printf("Data changed and lock released\n");

    return 0;
}
