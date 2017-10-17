#include "my_socket_api.h"

// BOM EXEMPLO
// http://www.binarytides.com/multiple-socket-connections-fdset-select-linux/

int main (int argc, char **argv) {
   int listenfd, connfd, n;
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

   f = fopen(argv[1], "a+");
   
   if (f == NULL) {
	    printf("Error opening file!\n");
	    exit(1);
   }

   // cria um socket TCP
   listenfd = Socket(AF_INET, SOCK_STREAM, 0);

   FD_ZERO(&rset);

   // espera por conexões de clientes indefinidamente
   for ( ; ; ) {
      FD_SET(fileno(f), &rset);
      FD_SET(listenfd, &rset);
      maxfdp1 = max(fileno(f), listenfd) + 1;
      Select(maxfdp1, &rset, NULL, NULL, NULL);

      if (FD_ISSET(listenfd, &rset)) {

         if (FD_ISSET(fileno(f), &rset)) {
	    receivedfile = fdopen(rset, "r");
	    
	    if(receivedfile != NULL){
	    	// TODO READ FROM SOCKET
	    }
         }
      }

   }

   // fecha arquivo de log
   fclose(f);

   return(0);
}

