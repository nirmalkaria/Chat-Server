// TCP client problem 3

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#define STDIN 0



main(int argc, char *argv[])
{

	int sockfd;
	int recv_by;
	char send_data[1000],recv_data[1000], time[1000],time2[1000];
	struct sockaddr_in client_addr; 
	struct timeval client_time,client_time2;
	long msec1,msec2,delta;
	time_t ctime,ctime2;
	unsigned short port; 
	char *IP;


	/*if (argc != 3)    
	{
		fprintf(stderr, "Insert %s <Server IP> <Server Port>\n", argv[0]);
		exit(1);
	}*/

	IP = "0.0.0.0";              
	port = 7000;


	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("Socket");
		exit(1);
	}

	client_addr.sin_family = AF_INET;     
	client_addr.sin_port = htons(port);   
	client_addr.sin_addr.s_addr = inet_addr(IP);
	bzero(&(client_addr.sin_zero),8);


	if (connect(sockfd, (struct sockaddr *)&client_addr,sizeof(struct sockaddr)) == -1) 
	{
		perror("Connect");
		exit(1);
	}
	 
	
		



	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(sockfd, &rfds);
	FD_SET(STDIN, &rfds);
	int nfds = getdtablesize(); 
	char name[1000];
	
	

	while(1)
	{
	bzero(time,1000);
	bzero(time2,1000);
	bzero(name,20);
FD_SET(sockfd, &rfds);
FD_SET(STDIN, &rfds);
	if(select(nfds, &rfds,NULL, NULL, (struct timeval *)0) < 0) {
		perror("select");
		return -1;
	}

	if(FD_ISSET(sockfd, &rfds))
	{
		if((recv_by=recv(sockfd,time2,1000,0))<=0)
		{

		if (recv_by == 0) 
		{
			printf("\n---------------------------------------\n\n");
			printf("Server hung up\nTrying to re-establish connection...\n\n");
			
			close(sockfd);
			if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
			{
				perror("Socket");
				exit(1);
			}

			client_addr.sin_family = AF_INET;     
			client_addr.sin_port = htons(port);   
			client_addr.sin_addr.s_addr = inet_addr(IP);
			bzero(&(client_addr.sin_zero),8);

			while (connect(sockfd, (struct sockaddr *)&client_addr,sizeof(struct sockaddr)) == -1) 
			{
				
				sleep(5);
			}
			printf("Connection Re-established...\n\n");
			printf("---------------------------------------\n\n");

		}else {
			perror("recv");
		}
		
		
		
	
		}
		else{
			time2[recv_by] = '\0';
			printf("\n\n");
			printf("%s" ,time2);}
	}

	if(FD_ISSET(STDIN, &rfds))
	{
		
		
		fgets(name,1000,stdin);
		name[1000]='\0';
		
		//printf("entered char %s\n",name);
	   send(sockfd,name,strlen(name), 0);


	}


	}




	

	


	



	


	printf("\n-------------------------------------------------\n");

	close(sockfd);

}


























