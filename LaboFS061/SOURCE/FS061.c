/** FS061 Ecrivez un programme qui affiche les caractéristiques des partitions primaires d'un disque en précisant si ces partitions sont bootables et vérifier que la signature de boot est présente.**/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


//Une entrée de partition fait toujours 16 bytes fill[12] + integer.
typedef struct ETP { //Entrée Table des Partitions
	//Bootable si 80h par convention.
	unsigned char bootable[1];
	unsigned char fill[11];
	//Cet entier nous rensiegne sur le nombre de blocs pour le FS.
	int s;
} __attribute__ ((packed)) ETP;	//éviter l'alignement des données.

//Structure type du mbr
typedef struct MBR {
	//Routine
	unsigned char code[442];
	unsigned char signature[4];
	//4 table des partitions primaires
	ETP TablePart[4];
	//Signature de boot 4 bytes: AA55h
	unsigned char bootable[2];
} __attribute__ ((packed)) MBR;

main (int argc, char * argv[]){
	int h,i,s;
	int boot;
	int type;
	MBR struMBR;
	if (argc != 2){
		printf("Ce programme s'utilise avec un seul argument : le pilote du disque\n");
		exit(EXIT_FAILURE);
	}
	//Ouverture en lecture seule au début du device.
	h=open(argv[1],O_RDONLY);
	if(h<0){
		printf("Ce pilote (%s) n'est pas valable ou vous n'êtes pas root\n",argv[1]);
		exit(EXIT_FAILURE);
	}

	printf("Partitions primaires de %s : \n",argv[1]);
	read(h,&struMBR,512);//Lecture du premier secteur du disque
	//Boucle de 4 (car 4 partitions primaires possible)
	for (i=0;i<4;i++){
		//Blocs de la partitions i.
		s=struMBR.TablePart[i].s;
		//Byte de boot.
		boot=struMBR.TablePart[i].bootable[0];
		//Type de FS.
		type=struMBR.TablePart[i].fill[3];

		//s>0 pour voir si il y a une taille (donc une partition)
		//boot==128 pour voir si le flag de boot est présent.
		//(s*512)/1024^2 pour passer de byte à megabyte.
		if(s > 0  && boot==128){ printf("Partition %d de %d secteurs (%d Mio) ID: %xh périphérique bootable: %xh\n",i+1,s,((s*512)/1024)/1024,type,boot);
		}else{	
			if(s > 0)	printf("Partition %d de %d secteurs (%d Mio) ID: %xh\n",i+1,s,((s*512)/1024)/1024,type);
		}
	}
	//Vérification de la signature de boot (55AA).
	printf("Signature de boot pour le MBR: %xh %xh\n",struMBR.bootable[0],struMBR.bootable[1]);	
	return EXIT_SUCCESS;
}

