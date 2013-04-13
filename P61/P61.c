/*
NOM     : Shell.c
CLASSE  : FS - LaboProcess 04-01
#OBJET  : réservé au makefile
AUTEUR  : J.C. Jaumain, 07/2011
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

//Fichier caché
#define BACKQUOTER ".bckq"


int main ()
{       int i,c,h,j;
	char * ligne;
	char *tokens[100];
	char *tokens2[100];
	char * temp;
	char * temp2;
	int BACKQUOTEF = 0;


	ligne=(char*)malloc(300);
	printf("$ ");
	fgets(ligne,256,stdin);
	while (strcmp(ligne,"exit\n"))
	{       i=0;
		j=0;
		tokens[i]=strtok(ligne," \n");
		while (tokens[i] != NULL) tokens[++i]=strtok(NULL," \n");
		if (fork()==0)
		{      

			//On regarde si ca commence par un backquote, si oui on se positione avec temp.
			if((i>1) && ((temp=strchr(tokens[i-1],'`')))){
				int h;	
				//Flag à 1 pour prévenir le shell du backquote.
				BACKQUOTEF=1;
				//On se positionne après le `
				temp=temp+1;
				//Fin de chaine je vire le dernier backquote avec un NULL
				temp[strlen(temp)-1]=0;
				temp[strlen(temp)+1]='\n';	
				//Ouverture du fichier caché en création (on recommence tjrs à 0);
				h=open(BACKQUOTER,O_WRONLY|O_CREAT|O_TRUNC,0644);
			
				//On vire le token ou il y a le backquote, 
				tokens[i-1]=0;	
				if (fork()==0)
				{
					//Ecran fichier
					dup2(h,1);
					close(h);

					//On fait des tokens avec ce qui a été mis dans les bacquote
					tokens2[j]=strtok(temp," \n");
					while (tokens2[j] != NULL) tokens2[++j]=strtok(NULL," \n");
					execvp(tokens2[0],tokens2);
					printf("Commande invalide\n");
					exit(1);
				}
				//Un wait par sécurité.
				wait(0);   
			}
			//Récupération du contenu dans le fichier si le flag est allumé.
			if(BACKQUOTEF==1){
				//on remet le flag à 0
				BACKQUOTEF=0;
				//On ouvre le fichier en lecture
				h=open(BACKQUOTER,O_RDWR,0644);
				int k=0;
				//On se positionne au début du fichier.
				lseek(h,0,SEEK_SET);
				//copie dans temp du contenu du fichier, qui a le résultat de l'exécution.
				//Saut de ligne inclus
				while(read(h,&c,1)!=NULL){
					temp[k]=c;
					k++;
				}
				close(h);

				//Je remplace les sauts de ligne avec un espace 0x20 en hexa.
				while((temp2=strchr(temp,'\n'))!=NULL){
					temp2[0]=0x20;
				}

				j=0;
				k=0;	

				//TOKENISING reprise de la tokenisation classique vue en cours.
				tokens2[j]=strtok(temp," \n");
				while (tokens2[j] != NULL) tokens2[++j]=strtok(NULL," \n");
		
				//On met dans le tokens[] principal les nouveaux arguments.	
				while(k<j){
				tokens[i-1]=tokens2[k];	
				k++;
				i++;
				}
				
				//Mettre un null à la fin
				tokens[i-1]=NULL;

				//On exécute le tout et PAN!
				execvp(tokens[0],tokens);
				printf("Commande invalide\n");
				exit(1);

			}
			//FIN DE MODIF.****--++++++

			//Exécution sans avoir le cas du backquote (cours).
			execvp(tokens[0],tokens);
			printf("Commande invalide\n");
			exit(1);
		}
		wait(0);
		printf("$ ");
		fgets(ligne,256,stdin);
	}
	exit(0);
}

