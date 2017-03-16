#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h> //defn of types like key_t
#include <sys/ipc.h> //interprocess communication access structure 
#include <sys/msg.h> //message queue structures 


struct my_msgbuf  //specifies the format of the message
    {
    long mtype; //type of the message, so process can select appropriate message(distinguishes them)
    char mtext[200]; //length of each IPC message in a queue is fixed
    };

int main(void)
    {
    struct my_msgbuf buf; //an instance of the message
    int msqid; //to obtain queue id
    key_t key; /*key to be passed to msgget() to identify a particular queue uniquely*/ 

    if ((key = ftok("clientmsq.c", 'B')) == -1)  //same as server
        {
        perror("ftok");
        exit(1); //exit(0) indicates successful program termination, While exit(1) (usually) indicates unsucessful termination.
        }

    /*The msgget() function initializes a new message queue. It can also return the message queue ID (msqid) of the queue 
    corresponding to the key argument. The value passed as the msgflg argument must be an octal integer with settings for
    the queue's permissions and control flags. IPC_CREAT tries to create the facility if it does not exist already.*/
    if ((msqid = msgget(key, 0644)) == -1)  /* connect to the queue */
        {
        perror("msgget");
        exit(1);
        }
    
    printf("Enter lines of text (in the form of no1 operator no2), ^D to quit:\n");

    buf.mtype = getpid(); /*the type of each message set as process PID to distunguish messages of multiple clients*/

    while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL)  //size fixed as defined
        {
        int len = strlen(buf.mtext);

        /* ditch newline at end, if it exists */
        if (buf.mtext[len-1] == '\n') buf.mtext[len-1] = '\0'; //in case size was less than specified and user pressed enter

        if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' that indicates end of array*/
            perror("msgsnd");
        }
    /* The msgctl() function alters the permissions and other characteristics of a message queue. The owner or creator of 
    a queue can change its ownership or permissions using msgctl() Also, any process with permission to do so can use
    msgctl() for control operations. IPC_RMID removes the message queue specified by the msqid argument. */

    /*Remove the message queue identifier msqid from the system and destroy any messages on the message queue. 
    Any threads that are waiting in msgsnd() or msgrcv() are woken up and msgsnd() or msgrcv() returns with a 
    return value of -1 and errno set to EIDRM. */
    if (msgctl(msqid, IPC_RMID, NULL) == -1) 
        {
        perror("msgctl");
        exit(1);
        }

    return 0;
    }
