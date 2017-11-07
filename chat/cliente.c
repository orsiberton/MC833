#include "my_socket_api.h"

int main(int argc, char **argv) {
   int    sockfd, n;
   char   recvline[MAXLINE], input[MAXLINE];
   struct sockaddr_in servaddr;

   // verifica se o host e a porta foram passados
   if (argc != 3) {
      perror("Host/Porta nao informados!");
      exit(1);
   }

   // cria um socket UDP
   sockfd = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

   // socket reutiliza o endereço IP
   Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));


   exit(0);
}
