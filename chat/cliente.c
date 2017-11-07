#include "my_socket_api.h"

int main(int argc, char **argv) {
   int    sockfd, n;
   char   recvline[MAXLINE], input[MAXLINE];;
   struct sockaddr_in servaddr;

   // verifica se o host e a porta foram passados
   if (argc != 3) {
      perror("Host/Porta do servidor nao informados!");
      exit(1);
   }

   // cria um socket UDP
   sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

   exit(0);
}
