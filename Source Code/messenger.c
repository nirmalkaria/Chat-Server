//TCP server problem 3

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
#include "chatroom.h"

int a[1000];
int count=0;
int serv_down=0,aaya=0;
int port;
int file_open=0;
char *conf_file;
char *root;
FILE *fileP = NULL;
void *tcpecho(void*);
int filesize;
int client_resolve(int);

void *make_file(void*);

int connection_wait(int sock)
{
	char *send_data;
	char *username;
	char recv_data [1000],path[1000];
	username=malloc(100);
	int recieved_by=0;
	bzero(recv_data,1000);
	bzero(username,100);
	
	send_data="Server > Waiting for connection\n";
	send(sock, send_data,strlen(send_data), 0);
	recieved_by = recv(sock,recv_data,3,0);



}


int client_connect(char *username,int u,int sock)
{
	char *send_data;
	char recv_data [1000],path[1000];
	char user[100];
	int recieved_by=0;
	bzero(recv_data,1000);
	
	int fsock;

	
	send_data="Server > Press: P - Initiate a personal chat\n         Press: G - Initiate a group chat\n	   ";    
	send(sock, send_data,strlen(send_data), 0);
	while(recieved_by<=0)
	{
	recieved_by = recv(sock,recv_data,1000,0);
	}
	//printf("recieved command %s\n",recv_data);
	//recv_data[recieved_by]='\0';
	if((strncmp(recv_data,"P",1))==0)
	{
		//printf("P compared\n");
		recieved_by=0;
	}
	else if((strncmp(recv_data,"G",1))==0)
	{
	count++;
	//printf("G compared\n");
	group_chat(sock,a,&count,username);
	}

	




//------------------------------------------------------
	FILE *fp;
	char *newline = "\n";

	if(file_open==0)
	{
		fp = fopen( "file.txt" , "w");
		file_open=1;
	}
	else
		fp = fopen( "file.txt" , "r+");
	

	if(fp!=NULL);
	else printf("error\n");
	fseek(fp, 0, SEEK_END);
   	filesize = ftell(fp);
	//printf("filesize %i\n\n\n",filesize);
	fwrite(username , 1 ,u-1, fp );
	fwrite(newline,1,strlen(newline),fp);
	fclose(fp);



//-------------------------------------------------------------





	send_data="Server > Enter the user you want to connect to\n";
	send(sock, send_data,strlen(send_data), 0);
	////printf("--------21\n");

	bzero(recv_data,1000);
	while(recieved_by<=0)
	{
	recieved_by = recv(sock,recv_data,1000,0);
	}
	recv_data[recieved_by-1]='\0';
	//printf("--------23\n");
	//printf("--------24\n");


	fp = fopen("file.txt", "r");
	if (fp == NULL)
	{
		//printf("Can't open username file!\n");
		exit(1);
	}
	//printf("--------25\n");



	do{
		if (fscanf(fp, "%s", user) != 1)
		{
			printf("fscan error\n");
			send_data="Server > No such user online, try again\n";
			send(sock, send_data,strlen(send_data), 0);
			recieved_by = recv(sock,recv_data,1000,0);
			recv_data[recieved_by-1]='\0';
			//printf("recieved data in while -->%s",recv_data);

		}
		//printf("Username found inside-->%s\n",user);
	}while(strcmp(user,recv_data));
	//printf("Username found -->%s\n",user);
	fclose(fp);
	


	bzero(path,1000);
	strcpy(path,root);
	strcat(path,user);
	strcat(path,"/");
	strcat(path,"sock.txt");
	//printf("socket path is%s\n",path);


	fp = fopen(path, "r");
	if (fp == NULL)
	{
		printf("Can't open socket file!\n");
		exit(1);
	}
	
		if (fscanf(fp, "%i", &fsock) != 1)
		{
			printf("Error in socket file on line 1!\n");
			return 0;
		}

	//printf("--------27\n");
	fclose(fp);

	return fsock;



}







void client_init(char *path,int sock)
{
	FILE *fp;


	if((fp = fopen(path, "w+" ))!=NULL)
	{
		fprintf(fp, "%i",sock);
	}
	else printf("error\n");
	fclose(fp);




}

int path_resolve(char *path1,char *recv_data1,int state,int sock)
{
	
	struct stat st = {0};

	if(state==1)
	
	{	//printf("path _resolve\n");
		strcat(path1,recv_data1);
		//printf("Path is %s\n",path1); 


		if (stat(path1, &st) == -1)
		{
			mkdir(path1,0700);     
			//printf("path created \n");
		}else 
		{
			//printf("Username already exists\n");  
			bzero(path1,1000);
			strcpy(path1,root);     
			return 0;		
		}

		return 1;
	}


	if(state==2)
	{
		strcat(path1,recv_data1);
		printf("Path is %s\n",path1); 


		if (stat(path1, &st) == -1)
		{
			//printf("No such user\n");
			bzero(path1,1000);
			strcpy(path1,root); 
			return 0;
		}

		else 
		{	
			char path[1000],pin[20];
			char *send_data,recv_data[1000];
			strcpy(path,path1);
			FILE *fp;
			int recieved_by=0;
			send_data="Server > Enter your pin number\n";
			send(sock, send_data,strlen(send_data), 0);
			recieved_by = recv(sock,recv_data,1000,0);
			recv_data[recieved_by-1]='\0';
			//printf("Pin recieved %s\n\n",recv_data);

			strcat(path,"/");
			strcat(path,"pin.txt");
			//printf("Pin path is %s\n",path);


			fp = fopen(path, "r");
			if (fp == NULL)
			{
				printf("Can't open username file!\n");
				exit(1);
			}
			//printf("--------25\n");



			do{
				if (fscanf(fp, "%s", pin) != 1)
				{
					////printf("fscan error\n");
					send_data="Server > Incorrect PIN, Try Again\n";
					send(sock, send_data,strlen(send_data), 0);
					recieved_by = recv(sock,recv_data,1000,0);
					recv_data[recieved_by-1]='\0';
					//printf("recieved pin data in while -->%s",recv_data);

				}
				//printf("pin found inside-->%s\n",pin);
			}while(strcmp(pin,recv_data));
			//printf("PIN found -->%s\n",pin);
			fclose(fp);
	
			return 2;




		}



	}

}

int main(int argc, char *argv[])
{
    int tsock,usock, ssock;
    char* currentLine = malloc(100);  
	root = malloc(100);
	conf_file = malloc(100); 
    unsigned short serverport;
    struct sockaddr_in server_addr,client_addr;    
    int size,value;
	pthread_t tid,tid2,sid;

	FILE *file_new = fopen("count_file.txt", "r");
	fscanf(file_new,"%i",&value);
	fclose(file_new);
	//printf("value is %i\n\n",value);

if (value!=0)
{
serv_down=1;
aaya=1;
}

	
pthread_create(&sid,NULL,&make_file,0);


	conf_file = "httpd.conf";
	fileP = fopen(conf_file, "r");
	if (fileP == NULL)
	{
		printf("Can't open configuration file!\n");
		exit(1);
	}
	if (fscanf(fileP, "%s %s", currentLine, root) != 2)
	{
		printf("Error in configuration file on line 1!\n");
		exit(1);
	}
	if (fscanf(fileP, "%s %i", currentLine, &port) != 2)
	{
		printf("Error in configuration file on line 2!\n");
		exit(1);
	}
	fclose(fileP);
	free(currentLine);
            


    serverport = port;  

    if ((tsock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
       	perror("tcp Socket");
       	exit(1);
    }


            
    server_addr.sin_family = AF_INET;         
    server_addr.sin_port =htons(serverport);     
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    bzero(&(server_addr.sin_zero),8); 


	int enable = 1;
	if (setsockopt(tsock, SOL_SOCKET, SO_REUSEADDR,&enable, sizeof(int)) < 0) 
	{
		printf("Failed to set sock option SO_REUSEADDR");
	}


    if (bind(tsock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))== -1) 
    {
	    perror("Unable to bind tsock");
	    exit(1);
    }



    if (listen(tsock, 2) == -1) 
    {
	    perror("Listen");
        exit(1);
    }


    printf("\n\n------------------------------------------\n");	
    printf("\nChat Server waiting for client \n");
    fflush(stdout);


    for(;;)
	{   sleep(2);
			size = sizeof(struct sockaddr_in);
			ssock = accept(tsock, (struct sockaddr *)&client_addr,&size);
			printf("\n TCP Server got a connection from (%s , %d)\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

			pthread_create(&tid,NULL,&tcpecho,(void*)&ssock);

	}	
pthread_join(sid,NULL);
    close(tsock);
    return 0;
}












void *tcpecho(void *arg)

{
	FILE *fp;
	int tcpsock = *(int*)arg;
	char *send_data, recv_data[1000],path[1000],r_data[1000],s_data[1000];
	char username [1000];
	int u;
//printf("TCP sock is %i\n",tcpsock);

	int recieved_by,path_state=1;
if(serv_down==0)
{
	int c = client_resolve(tcpsock);
	//printf("returned value is %i\n",c);
	bzero(recv_data,1000);	
	bzero(path,1000);
	strcpy(path,root);

	if(c==0)
	pthread_exit(NULL);

	if(c==1)
	{
		
		send_data="Server > welcome user\n         Enter a desired username\n";
		send(tcpsock, send_data,strlen(send_data), 0);
		recieved_by = recv(tcpsock,recv_data,1000,0);
		recv_data[recieved_by-1]='\0';

		do{
		path_state=path_resolve(path,recv_data,1,tcpsock);
		if(path_state==0)
		{
			send_data="Server > Username already EXISTS\n        Enter a different username\n";
			send(tcpsock, send_data,strlen(send_data), 0);
			bzero(recv_data,1000);
			recieved_by = recv(tcpsock,recv_data,1000,0);
			recv_data[recieved_by-1]='\0';

		}
		}while(path_state==0);	

		bzero(username,1000);
		strncpy(username,recv_data,recieved_by);
		u=recieved_by;	

		bzero(path,1000);
		strcpy(path,root); 
		strcat(path,recv_data);
		strcat(path,"/");
		strcat(path,"pin.txt");
		//printf("pin Path is %s\n",path);

		send_data="Server > choose a 5 digit numeric pin\n";  
		send(tcpsock, send_data,strlen(send_data), 0);
		recieved_by = recv(tcpsock,recv_data,1000,0);              
		recv_data[recieved_by] = '\0';
		printf("Numeric pin is - %s\n",recv_data);

		if((fp = fopen(path, "w+" ))!=NULL)
		{
			fprintf(fp, "%s",recv_data);
		}
		else printf("error in writing pin\n");
		fclose(fp);

		bzero(path,1000);
		strcpy(path,root); 
		strcat(path,username);
		strcat(path,"/");
		strcat(path,"sock.txt");
		//printf("sock Path is %s\n",path);
		

	}


	if(c==2)
	{
		send_data="Server > Welcome enter your username\n";
		send(tcpsock, send_data,strlen(send_data), 0);
		recieved_by = recv(tcpsock,recv_data,1000,0);
		recv_data[recieved_by-1]='\0';

		do{
		path_state=path_resolve(path,recv_data,2,tcpsock);

		if(path_state==0)
		{
			send_data="Server > No such USER, enter a valid USERNAME";
			send(tcpsock, send_data,strlen(send_data), 0);
			recieved_by = recv(tcpsock,recv_data,1000,0);
			recv_data[recieved_by-1]='\0';
		}
		}while(path_state==0);


		bzero(username,1000);
		strncpy(username,recv_data,recieved_by);
		u=recieved_by;
	
		strcat(path,"/");
		//strcat(path,recv_data);
		strcat(path,"sock.txt");
		//printf("check path\n");
		printf("Path is %s\n",path);




	}



	client_init(path,tcpsock);
	
}



	int fsock=0;
	int recv_by=0;
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(tcpsock, &rfds);
	int nfds = getdtablesize(); 
	char buff[1000];
	
if(serv_down==0)
{
	while(fsock<=0)
	{
		fsock=client_connect(username,u,tcpsock);
	}

}		

//printf("TCP sock is %i\n",tcpsock);

if(serv_down==1)
{
	char *fakeuser;
	if(tcpsock==4)
	{
send_data="Server > Please re enter your username\n";
		send(tcpsock, send_data,strlen(send_data), 0);
		recieved_by = recv(tcpsock,username,1000,0);
		username[recieved_by-1]='\0';
		
		fsock=5;
		//fakeuser="keval";
		//strcpy(username,fakeuser);
	}
	else{

send_data="Server > Please re-enter your username\n";
		send(tcpsock, send_data,strlen(send_data), 0);
		recieved_by = recv(tcpsock,username,1000,0);
		username[recieved_by-1]='\0';
		fsock=4;
		//fakeuser="sameer";
		//strcpy(username,fakeuser);
		}

}


	//printf("friend socket is %i\n",fsock);

	//printf("My socket is %i\n",tcpsock);

	while(1)
	{
		bzero(r_data,1000);
		bzero(s_data,1000);

		FD_SET(tcpsock, &rfds);

		if(select(nfds, &rfds,NULL, NULL, (struct timeval *)0) < 0) {
			perror("select");
			pthread_exit(NULL);
		}

		if(FD_ISSET(tcpsock, &rfds))
		{
			if((recv_by=recv(tcpsock,r_data,1000,0))>0)
			{
				r_data[recv_by] = '\0';
				
				bzero(buff,1000);
				strcpy(buff,username);
				strcat(buff," ---> ");
				strcat(buff,r_data);
				send(fsock,buff,strlen(buff), 0);

			}
		}




	}










	send(tcpsock, recv_data,strlen(recv_data), 0);	
	//printf("recieved data ---  %s  \n",recv_data);
	//printf("\n\n------------------------------------------\n");
				   
	close(tcpsock);

pthread_exit(NULL);

}





client_resolve(tempsock)
{
	char *buffer,re_buff[20];
	bzero(re_buff,20);

	buffer="Enter a: New User\n      b: Existing User\n";
	send(tempsock, buffer,strlen(buffer), 0);

	recv(tempsock,re_buff,3,0);
	//printf("Recieved variable is %s\n",re_buff);

	if((strncmp(re_buff,"a",1))==0)
	{
		//printf("New User\n");
		return 1;
	}
	else 
	if((strncmp(re_buff,"b",1))==0)
	{
		//printf("Existing User\n");
		return 2;
	}

	else 
	{
		//printf("Wrong input Please re-establish connection\n");
		return 0;
	}
	
}

//path_resolve(* path1,* recv_data1,state)


void *make_file(void *arg)	
{	
		//genrating random count using seconds

		 int x=0;
  		 FILE *file_new;
while(1)
 {		 
  		 x++;
		////printf("no is: %d\n", x);
  		sleep(2);
  		
  		
		
	    file_new = fopen("count_file.txt", "w+");
		
		fprintf(file_new, "%i",x);
		fclose(file_new);
		}
	}




