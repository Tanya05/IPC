#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //defines standard symbolic constants and types

int main(void)
	{
	/*Here we use the pipe system call pipe() to create a pipe. It takes an array of two integers, pfds, and if successful, 
      the array contains two file descriptors. pfds[0] is set up for reading, pfds[1] is set up for writing*/
	int pfds[2];
	pipe(pfds);
    
    /*After creating a pipe, the process typically spawns a new process (the child inherits open file descriptors) 
      A pipe can be used only between processes with a common ancestor.*/

	/*Here we are making the parent process execute ls -1 *.c, and child execute wc -L*/

	if(!fork()) //will return true (!0 = true) if its the parent process, else returns false in case of child process
		{
		/*The close() function shall deallocate the file descriptor indicated by fildes. To deallocate means to make the 
		file descriptor available for return by subsequent calls that allocate file descriptors. The dup() system call
		creates a copy of the file descriptor pfds, using the lowest-numbered unused file descriptor for the new descriptor.*/
		close(1); //close normal stdout
		dup(pfds[1]); //make stdout same as pfds[1] 
		close(pfds[0]);
		/*The  const  char *arg and subsequent arguments in the execlp() functions can be thought of  
		as  arg0,  arg1,  ...,  argn. Together  they represent the argument list available  to  the  executed  program. */
		//execlp("sh", "sh", "-c", "ls -1 *.c", NULL); //to invoke the shell and so * doesn't give error
		
		//* is a meta character won't be interpreted by the system call execlp
		system("ls -1 *.jpg"); //no need to invoke shell explicitly
		}
	else
		{
		close(0); //close normal stdin
		dup(pfds[0]); //make stdin same as pfds[0]
		close(pfds[1]);
		execlp("wc", "wc", "-l", NULL);
		}
	return 0;
	}
