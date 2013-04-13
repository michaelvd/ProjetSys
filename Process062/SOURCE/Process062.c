#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <fcntl.h>

#define ALIASRC ".aliasrc"

//Fonction qui exécute un alias donné et trouvé.
void alias2commande(int,const char **);

int main(){
	int i;
	int h;
	char * ligne;
	char * tokens[100];
	
	//DEBUT DE LA MODIF
	
	//On ouvre en création un fichier "caché" .aliasrc.
	h=open(ALIASRC,O_RDWR | O_CREAT, 0600);

	//La ligne de commande.
	ligne=(char*)malloc(300);

	//Le symbole du prompt.
	printf("$ ");
	//Lecture de la ligne.
	fgets(ligne,256,stdin);
	//Jusqu'à exit.
	while(strcmp(ligne,"exit\n")){
		i=0;
		//Découpage jusqu'au carriage return.
		tokens[i]=strtok(ligne," \n");
		while(tokens[i] != NULL) tokens[++i]=strtok(NULL," \n");
		//Dédoublement du process. 
		if(fork()==0){

			//Check si alias
			if(strcmp(tokens[0],"alias")==0){
				int j;
				//Essai regex.
				int reg;
				regex_t regi;
				//Définition de la syntaxe, de type var=commande [..]
				const char * regex1="^[a-z]+\=[-a-z0-9~!$%^&*_=+ \t}{\'?]+$";
				//On compile le regex.
				reg = regcomp(&regi,regex1,REG_EXTENDED);
				//Si ca marche pas.. (debug seulement).
				if(reg) {
					printf("REGEX COMPILE:Fail");
					exit(1);
				}
				//On regarde le token 1, le '=' doit etre collé à la variable de l'alias et à la commande.
				reg = regexec(&regi,tokens[1],0,NULL,0);
				//Si ca marche pas:
				if(reg==1){
					printf("Alias: Syntaxe incorrecte\n"); 
					exit(1);
				}	
				//Positionnement en fin de fichier.
				lseek(h,0,SEEK_END);
				//On écrit dans le fichier.
				for(j=1;j<i;j++){
					//A plat avec des espaces entre les tokens.
					write(h,tokens[j],strlen(tokens[j]));
					write(h," ",1);
				}
				write(h,"\n",1);
				exit(0);
			}
			//Affichage du fichier des alias. Utile.
			if(strcmp(tokens[0],"aliasdump")==0){
				char c;
				lseek(h,0,SEEK_SET);
				while(read(h,&c,1)!=NULL)printf("%c",c);
				printf("\n");		
				exit(0);
			}

			//On cherche à chaque fois un potentiel alias.
			alias2commande(h,tokens);	

			//FIN DE LA MODIF.

			//Exécution de la commande, execvp termine par exit(n) si la commande est trouvée.
			execvp(tokens[0],tokens);
			printf("Commande invalide\n");
			//Sortie en erreur si la commande n'est pas valide.
			exit(1);
		}
		//On attend la fin du fils avant de relancer le prompt (sinon l'ordonnanceur peut donner la main au prompt.
		wait(0);
		printf("$ ");
		fgets(ligne,256,stdin);
	}
	//On ferme le fichier.
	close(h);
	exit(0);
}


void alias2commande(int h,const char ** tokens){
	char * alias;
	char c=NULL;
	int i=-1;
	alias = (char *)malloc(256);

	//Positionnement en début de fichier.
	lseek(h,0,SEEK_SET);

	//On lit jusqu'à ce qu'on trouve par ligne.
	while(read(h,&c,1)!=NULL){
		i++;
		alias[i]=c;
		if(c=='\n'){
			if(strncmp(alias,tokens[0],strlen(tokens[0]))==0){
				//	printf("Found! : %s\n",alias);
				i=0;
				//On postionne alias après le caractère '='.
				while(alias[i]!='=')i++;
				alias=&alias[i+1];
				printf("%s\n",alias);

				//Procédure semblable à celle utilisée dans le main pour l'éxec.
				i=0;
				tokens[i]=strtok(alias," \n");
				while(tokens[i] != NULL) tokens[++i]=strtok(NULL," \n");

				//On exécute.
				execvp(tokens[0],tokens);
				printf("Commande invalide\n");
				//Sortie en erreur si la commande n'est pas valide.
				exit(1);
			}
			i=-1;
		}
	}
}
