#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
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
    char *token; 
    int no1, no2;
    char operator;
    FILE *output = fopen("output.txt", "w+");
    fclose(output);

    if ((key = ftok("clientmsq.c", 'B')) == -1) //ftok generates a random key, same for same arguments. Converts filename to key.
        {
        perror("ftok");
        exit(1); //exit(0) indicates successful program termination, While exit(1) (usually) indicates unsucessful termination.
        }

    /*The msgget() function initializes a new message queue. It can also return the message queue ID (msqid) of the queue 
    corresponding to the key argument. The value passed as the msgflg argument must be an octal integer with settings for
    the queue's permissions and control flags. IPC_CREAT tries to create the facility if it does not exist already.*/
    if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1)  /* create the queue */
        {
        perror("msgget");
        exit(1);
        }
    
    printf("Server: Ready to receive messages.\n");

    for(;;)  /* Server never quits! */
        {
        if (msgrcv(msqid, &buf, sizeof buf.mtext, 0, 0) == -1) 
            {
            perror("msgrcv");
            exit(1);
            }
        //printf("%li: \"%s\"\n", buf.mtype, buf.mtext);

    /*The server receives strings from the clients and then beaks them into numbers and operators using strtok that breaks
    a string into its components when separated by the specified character. Here it accepts string of the form 
    "no1 operator no2" and them separates the components based on the blank space between them. Rest are invalid strings.
    The variable token stores these components and returns them one by one.*/
        token = strtok(buf.mtext, " "); //using strtok to break the string into number and operator comppnents
        if(token == NULL)
            {
            output = fopen("output.txt", "a+");
            printf("%li: Empty String. Invalid Input.\n", buf.mtype);
            fprintf(output, "%li: Empty String. Invalid Input.\n", buf.mtype);
            fclose(output);
            }
        else
            {
            no1 = atoi(token); //to convert string to integer, as the first value of token is the first number
            token = strtok(NULL, " "); /*moves on to next component of token. Here we break token again using strtok and 
            when NULL and blank space are used as arguments, the next component is obtained.*/
            if(token == NULL)
                {
                output = fopen("output.txt", "a+");
                printf("%li: Invalid. Please try again.\n", buf.mtype);
                fprintf(output, "%li: Invalid. Please try again.\n", buf.mtype);
                fclose(output);
                }
            else
                {
                operator = token[0]; //since token is a string it acts as character array, thus index given
                token = strtok(NULL, " ");
                if(token == NULL)
                    {
                    output = fopen("output.txt", "a+");
                    printf("%li: Invalid. Please try again.\n", buf.mtype);
                    fprintf(output,"%li: Invalid. Please try again.\n", buf.mtype);
                    fclose(output);
                    }
                else
                    {
                    no2 = atoi(token);
                    //following performs different operations based on different operator values
                    if(operator == '+')
                        {
                        output = fopen("output.txt", "a+");
                        printf("%li: %d %c %d = %d\n",buf.mtype, no1, operator, no2, no1 + no2);
                        fprintf(output, "%li: %d %c %d = %d\n",buf.mtype, no1, operator, no2, no1 + no2);
                        fclose(output);
                        }
                    else if(operator == '-')
                        {
                        output = fopen("output.txt", "a+");
                        printf("%li: %d %c %d = %d\n",buf.mtype, no1, operator, no2, no1 - no2);
                        fprintf(output, "%li: %d %c %d = %d\n",buf.mtype, no1, operator, no2, no1 - no2);
                        fclose(output);
                        }
                    else if(operator == '*')
                        {
                        output = fopen("output.txt", "a+");
                        printf("%li: %d %c %d = %d\n",buf.mtype, no1, operator, no2, no1 * no2);
                        fprintf(output, "%li: %d %c %d = %d\n",buf.mtype, no1, operator, no2, no1 * no2);
                        fclose(output);
                        }
                    else if(operator == '/')
                        {
                        output = fopen("output.txt", "a+");
                        printf("%li: %d %c %d = %d\n",buf.mtype, no1, operator, no2, no1 / no2);
                        fprintf(output, "%li: %d %c %d = %d\n",buf.mtype, no1, operator, no2, no1 / no2);
                        fclose(output);
                        }
                    else //for computing power no1^no2
                        {
                        int base = 1, i=0;
                        for(i=no2; i>0; i--)
                            base = base*no1;
                        output = fopen("output.txt", "a+");
                        printf("%li: %d %c %d = %d\n",buf.mtype, no1, operator, no2, base);
                        fprintf(output, "%li: %d %c %d = %d\n",buf.mtype, no1, operator, no2, base);
                        fclose(output);
                        }
                    }
                }
            }
        }
    return 0;
    }