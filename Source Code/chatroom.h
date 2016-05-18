#ifndef chatroom_H_

#define chatroom_H_



/* protoypes for function*/

// sends messaged to all clients.

void send_to_all(int j, int i, int sockfd, int nbytes_recvd, char *recv_buf, fd_set *master,int a[],char username[]);



int group_chat( int sock,int a[],int *count,char username[]);

	



void send_recv(int i, fd_set *master, int sockfd, int fdmax,int a[],char username[]);

		

#endif
