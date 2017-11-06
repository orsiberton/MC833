#include "my_socket_api.h"

int main (int argc, char **argv) {
   int listenfd, connfd, n;
   unsigned int clientaddr_len;
   struct sockaddr_in servaddr, clientaddr;
   char buf[MAXLINE];
   char clientName[INET_ADDRSTRLEN];
   pid_t pid;

   // verifica se a porta foi passado por parametro
   if (argc != 2) {
      perror("Porta nao informada!");
      exit(1);
   }

   // cria um socket UDP
   listenfd = Socket(AF_INET, SOCK_DGRAM, 0);

   return(0);
}
