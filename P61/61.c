/*
NOM 	: Shell backquote
CLASSE 	: Process 61
# OBJET : réservé au makefile
AUTEUR	: Dubois Gauthier 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
int main ()
{
	int i ;
	int j ;
	int k;
	int l ;
	char * ligne ;
	char * tokens [100];
	ligne =(char*) malloc (300);
	printf ( "$" );
	fgets ( ligne ,256 , stdin );
	while ( strcmp ( ligne , "exit\n" ))
	{
		i =0;
		tokens [ i ]= strtok ( ligne , "\n" );
		while ( tokens [ i ] != NULL ) tokens [++ i ]= strtok ( NULL , "\n" );
		
		{
			if(tokens[j] !=NULL && tokens[j][0] == '`')
			{
				
				k=2;
				tokens[j][0] = tokens[j][1];
				while ( tokens[j][k] != '`')
				{
					tokens[j][k-1] = tokens[j][k];
					k++;
				}
				tokens[j][k-1]='\0';
				if ( fork () ==0 )
				{	
			printf ("je suis 7");	
					int h = open ("fichier", O_WRONLY | O_CREAT,0777 );
			printf ("je suis 8");	
					dup2(h,1);
			printf ("je suis 9");	
					close(h);
					execlp ( tokens[j],tokens[j] , 0);
					exit(1);
				}
				wait (0);
			}
			
		}			
		if ( fork ()==0)
		{
			l = 1;	
			int h = open ( "fichier" , O_RDONLY, 0777);
			char * buffer = tokens[0];
		 	while ( fgets( buffer[l],256,h) != NULL ){
				l++;
			}
			close(h);	
			execlp ( tokens[0] , buffer, 0  );
			printf ( "Commande invalide \n" );
		//	exit (1);
		}
		wait (0);
		printf ( " $ " );
		fgets ( ligne ,256 , stdin );
	}
	exit (0);
}

