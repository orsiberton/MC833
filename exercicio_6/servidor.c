#include "my_socket_api.h"

// BOM EXEMPLO
// http://www.binarytides.com/multiple-socket-connections-fdset-select-linux/

int main (int argc, char **argv) {
   int listenfd, connfd, n, port, max_sd, sd, new_socket, activity;
   unsigned int clientaddr_len, maxfdp1;
   struct sockaddr_in servaddr, clientaddr;
   char buf[MAXLINE], systembuf[MAXLINE], output[MAXLINE];
   char clientName[INET_ADDRSTRLEN];
   pid_t pid;
   bool isExiting = FALSE;
   FILE *f;
   FILE *receivedfile;
   fd_set rset;

   // verifica se o nome do arquivo foi passado por parametro
   if (argc != 2) {
      perror("Arquivo nao informado!");
      exit(1);
   }
   
   port = argv[1];

   f = fopen(argv[2], "a+");
   
   if (f == NULL) {
	    printf("Error opening file!\n");
	    exit(1);
   }

   // cria um socket TCP
   listenfd = Socket(AF_INET, SOCK_STREAM, 0);
   
   //type of socket created
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(atoi(port));
   
   // faz o bind do socket TCP com o host:porta escolhidos
   Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

   // ativa a socket para começar a receber conexões
   Listen(listenfd, LISTENQ);

   FD_ZERO(&rset);

   // espera por conexões de clientes indefinidamente
   for ( ; ; ) {
      FD_SET(listenfd, &rset);
      
      maxfdp1 = listenfd;
      
      activity = select(maxfdp1 + 1, &rset , NULL , NULL , NULL);
      
      if ((activity < 0) && (errno!=EINTR)) {
            printf("select error");
      }


	if (FD_ISSET(listenfd, &rset)) {
		if ((new_socket = accept(listenfd, (struct sockaddr *)&servaddr, (socklen_t*)&clientaddr_len))<0){
			perror("accept");
	                exit(EXIT_FAILURE);
		}
		
		if ((n = read(listenfd, buf, MAXLINE)) < 0) {
		       perror("read error");
		       exit(1);
     		}
		
		if( send(new_socket, buf, strlen(buf), 0) != strlen(buf) ) {
	                perror("send");
		}
	}

   }

   // fecha arquivo de log
   fclose(f);

   return(0);
}

