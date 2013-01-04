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

/* Declaration des fonctions */
int create_socket (int port);

int main (int argc, char**argv){
	

	return EXIT_SUCCESS;
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
