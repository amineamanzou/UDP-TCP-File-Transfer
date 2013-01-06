/*
 * File : client.c
 * Author : Amine Amanzou
 *
 * Created : 4th January 2013
 *
 * Under GNU Licence
 */

#include <stdio.h>
#include <stdlib.h>

// Time function, sockets, htons... file stat
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

// File function and bzero
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

/* Taille du buffer utilise pour envoyer le fichier
 * en plusieurs blocs
 */
#define BUFFERT 512

/* Commande pou génerer un fichier de test
 * dd if=/dev/urandom of=fichier count=8
 */

/* Declaration des fonctions*/
int duration (struct timeval *start,struct timeval *stop, struct timeval *delta);
int create_client_socket (int port, char* ipaddr);

struct sockaddr_in sock_serv;

int main (int argc, char**argv){
	struct timeval start, stop, delta;
    int sfd,fd;
    char buf[BUFFERT];
    off_t count=0, m,sz;//long
	long int n;
    int l=sizeof(struct sockaddr_in);
	struct stat buffer;
    
	if (argc != 4){
		printf("Error usage : %s <ip_serv> <port_serv> <filename>\n",argv[0]);
		return EXIT_FAILURE;
	}
    
    sfd=create_client_socket(atoi(argv[2]), argv[1]);
    
	if ((fd = open(argv[3],O_RDONLY))==-1){
		perror("open fail");
		return EXIT_FAILURE;
	}
    
	//taille du fichier
	if (stat(argv[3],&buffer)==-1){
		perror("stat fail");
		return EXIT_FAILURE;
	}
	else
		sz=buffer.st_size;
    
	//preparation de l'envoie
	bzero(&buf,BUFFERT);
    
    if(connect(sfd,(struct sockaddr*)&sock_serv,l)==-1){
        perror("conexion error\n");
        exit (3);
    }
	gettimeofday(&start,NULL);
    n=read(fd,buf,BUFFERT);
	while(n){
		if(n==-1){
			perror("read fails");
			return EXIT_FAILURE;
		}
		m=sendto(sfd,buf,n,0,(struct sockaddr*)&sock_serv,l);
		if(m==-1){
			perror("send error");
			return EXIT_FAILURE;
		}
		count+=m;
		//fprintf(stdout,"----\n%s\n----\n",buf);
		bzero(buf,BUFFERT);
        n=read(fd,buf,BUFFERT);
	}
	//read vient de retourner 0 : fin de fichier
	
	//pour debloquer le serv
	m=sendto(sfd,buf,0,0,(struct sockaddr*)&sock_serv,l);
	gettimeofday(&stop,NULL);
	duration(&start,&stop,&delta);
    
	printf("Nombre d'octets transférés : %lld\n",count);
	printf("Sur une taille total de : %lld \n",sz);
	printf("Pour une durée total de : %ld.%d \n",delta.tv_sec,delta.tv_usec);
    
    close(sfd);
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
int create_client_socket (int port, char* ipaddr){
    int l;
	int sfd;
    
	sfd = socket(PF_INET,SOCK_STREAM,0);
	if (sfd == -1){
        perror("socket fail");
        return EXIT_FAILURE;
	}
    
    //preparation de l'adresse de la socket destination
	l=sizeof(struct sockaddr_in);
	bzero(&sock_serv,l);
	
	sock_serv.sin_family=AF_INET;
	sock_serv.sin_port=htons(port);
    if (inet_pton(AF_INET,ipaddr,&sock_serv.sin_addr)==0){
		printf("Invalid IP adress\n");
		return EXIT_FAILURE;
	}
    
    return sfd;
}