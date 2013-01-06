/*
 * File : server.c
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
#include <time.h>
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
/* Taille de la file d'attente des clients */
#define BACKLOG 1

/* Commande pou génerer un fichier de test
 * dd if=/dev/urandom of=fichier count=8
 */

/* Declaration des fonctions*/
int duration (struct timeval *start,struct timeval *stop, struct timeval *delta);
int create_server_socket (int port);

struct sockaddr_in sock_serv,sock_clt;


int main(int argc,char** argv){
    int sfd,fd;
    unsigned int length=sizeof(struct sockaddr_in);
    long int n, m,count=0;
    unsigned int nsid;
    ushort clt_port;
    char buffer[BUFFERT],filename[256];
    char dst[INET_ADDRSTRLEN];
    
    // Variable pour la date
	time_t intps;
	struct tm* tmi;
    
    if(argc!=2) {
        perror("utilisation ./a.out <num_port> <file2send>\n");
        exit(3);
    }
    
    sfd = create_server_socket(atoi(argv[1]));
    
    bzero(buffer,BUFFERT);
    listen(sfd,BACKLOG);
    
    //Fonction qui attent la fonction connecte du client
    nsid=accept(sfd,(struct sockaddr*)&sock_clt,&length);
    if(nsid==-1){
        perror("accept fail");
        return EXIT_FAILURE;
    }
    else {
        if(inet_ntop(AF_INET,&sock_clt.sin_addr,dst,INET_ADDRSTRLEN)==NULL){
            perror("erreur socket");
            exit (4);
        }
        clt_port=ntohs(sock_clt.sin_port);
        printf("Debut de la connection pour : %s:%d\n",dst,clt_port);
        
        //Traitement du nom du fichier avec la date
        bzero(filename,256);
        intps = time(NULL);
        tmi = localtime(&intps);
        bzero(filename,256);
        sprintf(filename,"clt.%d.%d.%d.%d.%d.%d",tmi->tm_mday,tmi->tm_mon+1,1900+tmi->tm_year,tmi->tm_hour,tmi->tm_min,tmi->tm_sec);
        printf("Creating the copied output file : %s\n",filename);
        
        if ((fd=open(filename,O_CREAT|O_WRONLY,0600))==-1)
        {
            perror("open fail");
            exit (3);
        }
        bzero(buffer,BUFFERT);
        
        n=recv(nsid,buffer,BUFFERT,0);
        while(n) {
            if(n==-1){
                perror("recv fail");
                exit(5);
            }
            if((m=write(fd,buffer,n))==-1){
                perror("write fail");
                exit (6);
            }
            count=count+m;
            bzero(buffer,BUFFERT);
            n=recv(nsid,buffer,BUFFERT,0);
        }
        close(sfd);
        close(fd);
    }
    close(nsid);
    
    printf("Fin de la transmission avec %s.%d\n",dst,clt_port);
    printf("Nombre d'octets reçu : %ld \n",count);
    
    return EXIT_SUCCESS;
}

/* Fonction permettant la creation d'un socket et son attachement au systeme
 * Renvoie un descripteur de fichier dans la table de descripteur du processus
 * bind permet sa definition aupres du systeme
 */
int create_server_socket (int port){
    int l;
	int sfd;
    int yes=1;
    
	sfd = socket(PF_INET,SOCK_STREAM,0);
	if (sfd == -1){
        perror("socket fail");
        return EXIT_SUCCESS;
	}
    /*SOL_SOCKET : To manipulate options at the sockets API level
     *SO_REUSEADDR : Lorsqu'on doit redémarrer un serveur après un arrêt brutal cela peut servir
     * à ne pas avoir une erreur lors de la création de la socket (la pile IP du système n'a pas
     * toujours eu le temps de faire le ménage).
     * Cas où plusieurs serveurs qui écoutent le même port ... (?)
     */
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)) == -1 ) {
        perror("setsockopt erreur");
        exit(5);
    }
    
    //preparation de l'adresse de la socket destination
	l=sizeof(struct sockaddr_in);
	bzero(&sock_serv,l);
	
	sock_serv.sin_family=AF_INET;
	sock_serv.sin_port=htons(port);
	sock_serv.sin_addr.s_addr=htonl(INADDR_ANY);
    
	//Affecter une identité au socket
	if(bind(sfd,(struct sockaddr*)&sock_serv,l)==-1){
		perror("bind fail");
		return EXIT_FAILURE;
	}
    
    
    return sfd;
}