#include "my_socket_api.h"

int main (int argc, char **argv) {
   int listenfd, n;
   unsigned int clientaddr_len;
   struct sockaddr clientaddr;
   char buf[MAXLINE];

   // verifica se a porta foi passado por parametro
   if (argc != 2) {
      perror("Porta nao informada!");
      exit(1);
   }

   // cria um socket UDP
   listenfd = Socket(AF_INET, SOCK_DGRAM, 0);

   for(;;){
      n = recvfrom(listenfd, buf, MAXLINE, 0, &clientaddr, &clientaddr_len);
   }

   return(0);
}
