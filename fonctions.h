#define RCVSIZE 1024000
#define DATA "toto.txt"
#define RECEP "reception.txt"
#define TAILLE 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/stat.h>


void fichier (struct sockaddr_in adresse, int desc){
	char c;
	FILE *fp;
	int i = 0;
  char buffer[11];
	//char buffer[RCVSIZE];
	//Lecture du fichier initial
	fp = fopen(DATA, "r");
	if(fp == NULL){
		printf("\n Erreur : Impossible de lire le fichier %s\n", DATA);
	}

	while (( c = fgetc (fp)) !=  EOF){
    if(i==10){
			buffer[10] = '\0';
      i=0;
      printf("%s", buffer);
      sendto(desc, buffer, sizeof(buffer), 0, (struct sockaddr*)&adresse, sizeof(adresse));

    }
  	buffer[i] = c;
    i++;
	}
	//strcpy(tab, buffer);
  fclose(fp);
}

void fichierBinaire (struct sockaddr_in adresse, int desc){
	char c;
	FILE *fp;
	int i;
	int j = 0;
  char msg[TAILLE+1];
	char buffer[RCVSIZE];
	struct stat st;
	int size;

	//Lecture du fichier initial
	fp = fopen(DATA, "rb");
	if(fp == NULL){
		printf("\n Erreur : Impossible de lire le fichier %s\n", DATA);
	}

	stat(DATA, &st);
	size = st.st_size;
	printf("%d\n", size);

	fread(buffer, sizeof(char), size, fp);
	for(i =0; i<size; i++){
		if(i!= 0 && i%TAILLE == 0){
			msg[j] = buffer[i];
			//msg[j] = itoa(i);
			//printf("%c\n", msg[j]);
			//msg[TAILLE]='\0';
			//printf("%s", msg);

			sendto(desc, msg, sizeof(msg), 0, (struct sockaddr*)&adresse, sizeof(adresse));
			//printf("%d\n",i+TAILLE);
			memset(msg,0,TAILLE+1);
			j = 0;
		}else{
			msg[j] = buffer[i];
			j++;
		}
		if(i+TAILLE >= size){
			printf("ici2\n");
			j = 0;
			char msg2 [size-i];
			for(i; i<size; i++){
					msg2[j] = buffer[i];
					j++;
				}
				fflush(stdout);
				//printf("%s", msg);
				//printf("ici1\n");
				printf("%s\n",msg2);
				sendto(desc, msg2, sizeof(msg2), 0, (struct sockaddr*)&adresse, sizeof(adresse));
				//char fin[TAILLE+1]= "000000000000000000000";
				//printf("%s\n",fin);
				//sendto(desc, fin, sizeof(fin), 0, (struct sockaddr*)&adresse, sizeof(adresse));
				//printf("OK2\n");
		}
	}

	//strcpy(tab, buffer);
  fclose(fp);
}


void receptionBinaire (char* buffer){

		FILE *fp;
		fp = fopen(RECEP, "ab+");
		fwrite(buffer, sizeof(char),TAILLE,fp);
		fclose(fp);

}

char* numSeq (int* a, int* b, int* c, int* d, int* e, int* f, char* msg){

		if (*f == 10){
			*f = 0;
			*e += 1;
		}
		if (*e == 10){
			*e = 0;
			*d += 1;
		}
		if (*d == 10){
			*d = 0;
			*c += 1;
		}
		if (*c == 10){
			*c = 0;
			*b += 1;
		}
		if (*b == 10){
			*b = 0;
			*a += 1;
		}
		if (*a == 10){
			//Trop de num de numSeq
			printf("Trop de num de seq\n");
		}
		msg[0]= *a + '0';
		msg[1]= *b + '0';
		msg[2]= *c + '0';
		msg[3]= *d + '0';
		msg[4]= *e + '0';
		msg[5]= *f + '0';
		return msg;
}


void envoieFichierbinaire(struct sockaddr_in adresse, int desc, socklen_t alen){

	FILE *fp;
	int seq = 0;
	int j = 6;
	int i = 0;

	//Initialistaion variables pour numéro de séquence
	int* a;
	a = (int *)malloc(sizeof(int));
	*a = 0;

	int* b;
	b = (int *)malloc(sizeof(int));
	*b = 0;

	int* c;
	c = (int *)malloc(sizeof(int));
	*c = 0;

	int* d;
	d = (int *)malloc(sizeof(int));
	*d = 0;

	int* e;
	e = (int *)malloc(sizeof(int));
	*e = 0;

	int* f;
	f = (int *)malloc(sizeof(int));
	*f = 1;

  char msg[TAILLE+6]; //Message envoyé



	char msgrcv[TAILLE]; //ACK envoyé par le client reçu

	char buffer[RCVSIZE]; //Pour récupérer le texte du fichier toto.txt
	struct stat st;
	int size;

	//Lecture du fichier initial
	fp = fopen(DATA, "rb");
	if(fp == NULL){
		printf("\n Erreur : Impossible de lire le fichier %s\n", DATA);
	}

	stat(DATA, &st);
	size = st.st_size;
	printf("taille fichier : %d\n", size);

	fread(buffer, sizeof(char), size, fp);
	int nbSeq = size/TAILLE+1;
	printf("Nombre de Sequence : %d\n", nbSeq);

	for(int v = 1; v<nbSeq+1;v++){
		numSeq(a, b, c, d, e, f, msg); //On met le numéro de seq au début

		for(i; i<TAILLE*v; i++){
				msg[j] = buffer[i]; //On remplit msg avec le texte
				j++;
		}

		printf("%s\n", msg);

		sendto(desc, msg, sizeof(msg), 0, (struct sockaddr*)&adresse, sizeof(adresse)); //On envoie au client

		recvfrom(desc, msgrcv, sizeof(msgrcv), 0, (struct sockaddr*)&adresse, &alen);
		printf("%s\n", msgrcv);
		fflush(stdout);

		*f+=1; //On incrémente le numéro de séquence
		memset(msg,0,TAILLE+6); //On reset msg
		j = 6;
	}

}
