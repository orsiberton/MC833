#include "my_socket_api.h"

int main(int argc, char **argv) {
   int    sockfd, n, data_sent = 0, data_received = 0;
   char   recvline[MAXLINE], input[MAXLINE];
   struct sockaddr_in servaddr;
   fd_set selectfd;

   // verifica se o host e a porta foram passados
   if (argc != 3) {
      perror("Host/Porta nao informados!/n");
      exit(1);
   }

   // cria um socket TCP
   sockfd = Socket(AF_INET, SOCK_STREAM, 0);

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
   Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

   FD_ZERO(&selectfd);

   // le entrada do cliente
   for ( ; ; ) {
     // faz o set no stdin e no socket
     FD_SET(fileno(stdin), &selectfd);
     FD_SET(sockfd, &selectfd);

     // faz o select em selectfd
     Select(FD_SETSIZE, &selectfd, NULL, NULL, NULL);

     // verifica se o stdin foi setado
     if (FD_ISSET(fileno(stdin), &selectfd)) {
       // le do servidor
       n = read(fileno(stdin), recvline, MAXLINE);

       // caso algo tenha sido enviado
       if (n > 0) {
         data_sent =+ n;
         write(sockfd, recvline, n);
       }
     }

     // verifica se o socket foi setado
     if (FD_ISSET(sockfd, &selectfd)) {
       n = read(sockfd, input, MAXLINE);
       data_received =+ n;
       write(fileno(stdout), input, n);
     }

     // caso todos os bytes forem enviados, encerra o cliente
     if (data_received == data_sent) {
       break;
     }
   }

   return(0);
}
