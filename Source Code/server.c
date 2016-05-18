#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/param.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/wait.h>





void slip(int s)
{

sleep(s);

}
int main(int argc, char *argv[])
{
    int size, value1=0;
    
    pid_t server_pid;
    FILE *file_new;
   	file_new = fopen("count_file.txt", "w+");
	fprintf(file_new, "%i",0);
	close(file_new);
    sleep(2);






	if (fork()==0)
	    {	

			//printf("value in child is %i\n\n",value1);
			printf("\n\nInitiating a new Server....\n\n");   
			//execve("./test1",NULL,NULL);  
			system("gnome-terminal -e ./messenger"); 
	
	    }

	else{
int value2,value3;
while(1)
{
		value2=0;value3=0;
	   	file_new = fopen("count_file.txt", "r");
	   	fscanf(file_new,"%i",&value2);
		fclose(file_new);
		

        slip(10);
		file_new = fopen("count_file.txt", "r");
	   	fscanf(file_new,"%i",&value3);
		fclose(file_new);
		
		if (value2 == value3)
		{
			//printf("---- %i  %i---",value2,value3);
			printf("Server has stopped working... Starting a backup server\n\n");
			system("gnome-terminal -e ./messenger");
		
		}
			//printf("---- %i  %i---\n",value2,value3);
	
}
}

}

