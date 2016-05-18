#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "chatroom.h"

#define BUFSIZE 1024
int count2=0;
send_all(char room[],int sock,int a[])
{
	int z;
	for(z=0;z<=count2;z++)
	{
		if(sock==a[z])
	printf("\n");
	else
	{
		if (send(a[z], room, strlen(room), 0) == -1)
		{
			perror("send");
		}
	}

	}


}

void send_to_all(int j, int i, int sockfd, int nbytes_recvd, char *recv_buf, fd_set *master,int a[],char username[])
{	


	char room[1000];
	strcpy(room, username);
	strcat(room," ---> ");
	strcat(room,recv_buf);

	


	if(i==a[j])
	printf("\n");
	else
	{
		if (send(a[j], room, strlen(room), 0) == -1)
		{
			perror("send");
		}
	}


}
		
void send_recv(int i, fd_set *master, int sockfd, int fdmax,int a[],char username[])
{
	int nbytes_recvd, j;
	char recv_buf[BUFSIZE], buf[BUFSIZE];
	//printf("send_recv-----1\n");
	if ((nbytes_recvd = recv(i, recv_buf, BUFSIZE, 0)) <= 0)
	 {
		if (nbytes_recvd == 0) {
			printf("socket %d hung up\n", i);
		}else {
			perror("recv");
		}
		
		close(i);
		FD_CLR(i, master);
	}else 
		{ 
			for(j = 0; j <= count2; j++)
			{
				send_to_all(j, i, sockfd, nbytes_recvd, recv_buf, master,a,username);
			}
		}	

}
		

	

int group_chat(int client_sock,int a[],int *count,char username[])
{
	char room[1000];


	count2=*count;
	count2--;

	printf("\n");
	//printf("count is %i\n",count2);
	
	strcpy(room,username);
	strcat(room," is connected to the chatroom\n");
	printf("%s \n",room);

	a[count2]=client_sock;

	send_all(room,client_sock,a);


	fd_set master;
	fd_set read_fds;
	int fdmax, i;
	int sockfd= 0;
	
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	
	FD_SET(client_sock, &master);
	//printf("sockfd %i\n",client_sock);
	fdmax = client_sock;
	while(1)
	{
		read_fds = master;
		if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
		{
			perror("select");
			exit(4);
		}
		
		for (i = 0; i <= fdmax; i++)
		{
			
			if (FD_ISSET(i, &read_fds)){
				{
					send_recv(i, &master, client_sock, fdmax,a,username);
				}

			}
		}
}
	return 0;
}
