/*
NOM	:	IPC038S.c
CLASSE	:	IPC
AUTEUR	:	Vandycke Michaël
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
main()
{       struct sockaddr_in bd,s;
	int sock,h,len;
	char buf[255] = "";
	int total=0;
	int i;
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock==-1) { perror("SOCKET"); exit(1);}
	bd.sin_family = AF_INET;
	bd.sin_port   = htons(5938);
	if (! inet_aton("127.0.0.1",&bd.sin_addr.s_addr)) { perror("INET_ATON"); exit(1);}
	if (bind(sock, &bd, sizeof(bd))==-1) { perror("BIND"); exit(1);}
	listen(sock,1);
	h = accept(sock,&s,&len);
	read(h, &buf, 255);
	printf("SERVEUR: message reçu : %s\n",buf);
	printf("SERVEUR: Longueur du message : %d\n",strlen(buf));
	 for(i=0;i<strlen(buf);i++){
		total=total+(buf[i] - '0');
	}
	sprintf(buf,"%d",total);
	write(h,&buf);
	close(sock);
}

