/*
 * File : client.c
 * Author : Amine Amanzou
 * 
 * Created : 4th January 2013
 * 
 * Sous license GNU
 */

#include <stdio.h>
#include <stdlib.h>

/* Taille du buffer utilise pour envoyer le fichier                            
 * en plusieurs blocs
 */                                                                           
#define BUFFERT 512                                                            
                                                                               
/* Commande pou génerer un fichier de test                                     
 * dd if=/dev/urandom of=fichier count=8
 */

/* Declaration des fonctions*/
int duration (struct timeval *start,struct timeval *stop, struct timeval *delta);       
int create_socket (int port);

int main (int argc, char**argv){
	

	return EXIT_SUCCESS;
}

/* Fonction permettant le calcul de la durée de l'envoie */
int duration (struct timeval *start,struct timeval *stop,struct timeval *delta)
{
        suseconds_t microstart, microstop, microdelta;

        microstart = (suseconds_t) (100000*(start->tv_sec))+ start->tv_usec;
        microstop = (suseconds_t) (100000*(stop->tv_sec))+ stop->tv_usec;
        microdelta = microstop - microstart;

        delta->tv_usec = microdelta%100000;
        delta->tv_sec = (time_t)(microdelta/100000);

        if((*delta).tv_sec < 0 || (*delta).tv_usec < 0)
                return -1;
        else
                return 0;
}

/* Fonction permettant la creation d'un socket                                 
 * Renvoie un descripteur de fichier
 */
int create_socket (int port){
        struct sockaddr_in sock_serv;
	int sfd;

	sfd = socket();
	if (sfd == -1){

	}

        return sfd;
}
