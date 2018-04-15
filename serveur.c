
#include "fonctions.h"

#define RCVSIZE 1024000
#define TAILLE 20
#define DATA "toto.txt"
#define RECEP "reception.txt"


int main (int argc, char *argv[]) {

	int cont = 1;


	if (argc == 2){

		struct sockaddr_in adresse, client, adresse2;
		int valid= 1;
		socklen_t alen= sizeof(client);
		char buffer[RCVSIZE];
		char msg[TAILLE+1];

		int connexion = 0;
		int pid=1;

		//create socketUDP
		int desc = socket(AF_INET, SOCK_DGRAM, 0);
		int desc2 = socket(AF_INET, SOCK_DGRAM, 0);

		// handle error
		if (desc2 < 0) {
			perror("cannot create socket\n");
			return -1;
		}
		if (desc < 0) {
			perror("cannot create socket\n");
			return -1;
		}

		setsockopt(desc2, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));
		setsockopt(desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

		//Pour UDP
		adresse2.sin_family= AF_INET;
		adresse2.sin_port= htons(atoi(argv[1]));
		adresse2.sin_addr.s_addr= htonl(INADDR_ANY);

		adresse.sin_family= AF_INET;
		adresse.sin_port= htons(6666);	//Num de port choisi pour com 6666
		adresse.sin_addr.s_addr= htonl(INADDR_ANY);


		//Liaison adresse ä socket UDP
		if (bind(desc2, (struct sockaddr*) &adresse2, sizeof(adresse2)) == -1) {
			perror("Bind fail\n");
			close(desc2);
			return -1;
		}

		if (bind(desc, (struct sockaddr*) &adresse, sizeof(adresse)) == -1) {
			perror("Bind fail\n");
			close(desc);
			return -1;
		}


		while(connexion == 0){

			//Processus pere pour établir la connexion avec client
			if(pid>0){
				int msgSize2 = recvfrom(desc2, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&client, &alen);

				if(strncmp(buffer, "SYN",3) == 0){
					sendto(desc2, "SYN-ACK6666", strlen("SYN-ACK6666"), 0, (struct sockaddr*)&client, sizeof(client)); //Envoie num de port et SYN
				}
				recvfrom(desc2, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&client, &alen);
				if(strncmp(buffer, "ACK",3) == 0){
					printf("Connexion établie\n");
				}
				pid = fork();
			//	int compteur;
			//Processus fils
			}else{
				int i = 0;

					while (cont == 1) {

						int msgSize2 = recvfrom(desc, msg, sizeof(msg), 0, (struct sockaddr*)&adresse, &alen); //Reception du nom du fichier à transférer
						fflush(stdout);
						printf("%s\n", msg);

						//Envoie fichier
						envoieFichierbinaire(adresse, desc, alen);
						/*

					//UDP
						char msg2[9] = "000001FIN";
						int msgSize2 = recvfrom(desc, msg, sizeof(msg), 0, (struct sockaddr*)&adresse, &alen);
						fflush(stdout);
						printf("%s\n", msg);
						//fichierBinaire(adresse, desc);

						sendto(desc, msg2, sizeof(msg2), 0, (struct sockaddr*)&adresse, sizeof(adresse));
						//char msg3[3] = "FIN";

						recvfrom(desc, msg, sizeof(msg), 0, (struct sockaddr*)&adresse, &alen);
						fflush(stdout);
						printf("%s\n", msg);

					//	sendto(desc, msg3, sizeof(msg3), 0, (struct sockaddr*)&adresse, sizeof(adresse));
*/
						cont = 0;
					}

					connexion = 1;

					//close(desc);

			}
		}


	}else{
		printf("Wrong arguments... Try again\n");
	}







	return 0;
}
