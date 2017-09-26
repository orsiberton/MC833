#include "my_socket_api.h"

int main(int argc, char **argv) {
   int    sockfd, n;
   unsigned int servaddr_len;
   char   recvline[MAXLINE + 1];
   struct sockaddr_in servaddr;

   // verifica se o host e a porta foram passados
   if (argc != 3) {
      perror("Host/Porta nao informados!");
      exit(1);
   }

   // cria um socket TCP
   sockfd = Socket(AF_INET, SOCK_STREAM, 0);
   /*
   if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket error");
      exit(1);
   }
   */

   // configura os parâmetros da conexão
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr(argv[1]);
   servaddr.sin_port   = htons(atoi(argv[2]));

   if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
      perror("inet_pton error");
      exit(1);
   }

   // abre a conexão com o servidor
   if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
      perror("connect error");
      exit(1);
   }

   // le o que foi recebido através do socket e imprime o conteúdo
   while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
      recvline[n] = 0;
      if (fputs(recvline, stdout) == EOF) {
         perror("fputs error");
         exit(1);
      }
   }

   if (n < 0) {
      perror("read error");
      exit(1);
   }

   // imprime dados do socket
   servaddr_len = sizeof(struct sockaddr_in);
   if (getsockname(sockfd, (struct sockaddr *) &servaddr, &servaddr_len) == -1) {
     perror("getsockname() failed");
     return -1;
   }
   printf("IP address do socket: %s\n", inet_ntoa(servaddr.sin_addr));
   printf("Client port do socket: %d\n", (int) ntohs(servaddr.sin_port));

   exit(0);
}
