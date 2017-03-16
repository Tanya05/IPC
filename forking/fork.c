#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h> //declarations for waiting, like exit status of process child etc
#include <unistd.h> //standard symbolic constants and types (POSIX and all) : see what POSIX and all are (ANSI)
#include <sys/types.h> //defn for types like pid_t etc
#include <errno.h> /*The errno.h header file of the C Standard Library defines the integer variable errno, 
which is set by system calls and some library functions in the event of an error to indicate what went wrong. 
(The program returns an error along with errno set to an error constant defined in this library.) 
This macro expands to a modifiable lvalue of type int, therefore it can be both read and modified by a program.*/

/*An lvalue (locator value) represents an object that occupies some identifiable location in memory (i.e. has an address).
rvalues are defined by exclusion, by saying that every expression is either an lvalue or an rvalue. 
Therefore, from the above definition of lvalue, an rvalue is an expression that does not represent 
an object occupying some identifiable location in memory.*/

 int gvar = 0; //global variable
 int main()
 	{
	pid_t childPID;
	/*Your program should include the header files unistd.h and sys/types.h to use these functions. 
 Data Type: pid_t. The pid_t data type is a signed integer type which is capable of representing 
 a process ID. In the GNU C Library, this is an int . Function: pid_t getpid (void)*/
	int lvar=0;
	childPID = fork();
	if(childPID >= 0) //fork successful
		{
		if(childPID == 0) //child process
			{
			gvar++;
			lvar++;
			printf("Child Process:: global variable: %d, local variable: %d\n", gvar, lvar);
			}
		else //parent process
			{
			gvar++;
			lvar++;
			printf("Parent Process:: global variable: %d, local variable: %d\n", gvar, lvar);
			}
		} 
	else //fork failed
		{
		printf("Fork Failed.\n");
		return 1;
		}	
 	return 0;
 	}