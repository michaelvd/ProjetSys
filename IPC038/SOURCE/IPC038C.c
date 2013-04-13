/*
NOM	:	IPC038C.c
CLASSE	:	IPC 
AUTEUR	:	Vandycke Michaël
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void usage();

main(int argc, char * argv[]){
	char  num[10];
	int sd,res;
	struct sockaddr_in sockstr;
	struct in_addr adresse;
	sd=socket(PF_INET,SOCK_STREAM,0);
	if(sd==-1){	perror("SOCKET"); exit(1);}
	sockstr.sin_family=AF_INET;
	sockstr.sin_port=htons(5938);
	if(argc != 2){ usage(); exit(1); }
	if(!inet_aton(argv[1],&adresse)) { perror("INET_ATON"); exit(1);}
	sockstr.sin_addr=adresse;
	if(connect(sd,&sockstr,sizeof(sockstr))==-1){ perror("CONNECT"); exit(1);}
	printf("Saisissez un nombre (MAX 10):\n");
	gets(&num);
	write(sd,&num);
	read(sd,&num,255);
	printf("Somme totale: %s\n",num);
	close(sd);
}

void usage(){
	printf("Usage:		IPC038C IPADDRESS\n");
	printf("Exemple:	IPC038C 127.0.0.1\n");
}
