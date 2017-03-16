#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024  /* make it a 1K shared memory segment */

int main(int argc, char *argv[])
    {
    key_t key; //key_t data type defined in sys/types.h, used to declare key values
    int shmid;
    char *data;
    int mode;
    pid_t childPID;

    if (argc > 2) //more than one input given by user
        { 
        fprintf(stderr, "usage: shmdemo [data_to_write]\n");
        exit(1);
        }

    /* make the key: The  ftok() function uses the identity of the file named by the given pathname (which must refer 
    to an existing, accessible file) and the least significant 8 bits of proj_id (which must be nonzero) to generate a 
    key_t type System V IPC  key,  suitable  for  use  with msgget(2), semget(2), or shmget(2).*/
    if ((key = ftok("shmem.c", 'R')) == -1) 
        { 
        perror("ftok");
        exit(1);
        }

    /* connect to (and possibly create) the segment. IPC_CREAT creates if it doesn't already exist. */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) 
        {
        perror("shmget");
        exit(1);
        }

    /* attach to the segment to get a pointer to i. Once a shared memory segment has been created, a process attaches it to 
    its address space by calling shmat(). shmat() returns a pointer to shared memory segment if OK, -1 on error.  
    
    void *shmat(int shmid, const void *shmaddr, int shmflg);
    
    sIf shmaddr is NULL, the system chooses a suitable (unused) address at which to attach the segment.*/

    data = shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1)) 
        {
        perror("shmat");
        exit(1);
        }

    childPID = fork();

    if(childPID >= 0)//fork successful
        {
        if (childPID == 0) //child process 
            {
            /* read or modify the segment, based on the command line: */
            if (argc < 2) 
                printf("Too few arguments. Please give some input.\n");
            else
                {
                printf("Child writing to segment: \"%s\"\n", argv[1]);
                strncpy(data, argv[1], SHM_SIZE); //string being copied to data is some input is available from command line
                }
            }
        else //parent process
                {
                sleep(1); //waiting so that child process gets time to read input
                printf("Parent reads from segment: \"%s\"\n", data); 
                }
        }
    else //fork failed
        {
        printf("Fork Failed.\n");
        return 1;
        }   

    /* detach from the segment. Tshmdt() detaches the shared memory segment located at the address specified  by  shmaddr  
    from the address space of the calling process.  The to-be-detached segment must be currently attached with shmaddr equal 
    to the value returned by the attaching shmat() call.

       int shmdt(const void *shmaddr); */ 
    if (shmdt(data) == -1) 
        {
        perror("shmdt");
        exit(1);
        }
    return 0;
    }
