#include "my_socket_api.h"

int main(int argc, char **argv) {
   int    sockfd, n;
   char   recvline[MAXLINE], input[MAXLINE];;
   struct sockaddr_in servaddr;
   FILE *in, *out;

   // verifica se o host e a porta foram passados
   if (argc != 5) {
      perror("Host/Porta nao informados!");
      exit(1);
   }

   in = fopen(argv[3], "r");
   out = fopen(argv[4], "a");

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

    while (fgets(input, MAXLINE, in) != NULL) {
     write(sockfd, input, strlen(input));

     // termina execução do cliente caso o cliente queira
     if (isExit(input)) {
       break;
     }

     // le o que foi recebido através do socket e imprime o conteúdo
     if ((n = read(sockfd, recvline, MAXLINE)) < 0) {
       perror("read error");
       exit(1);
     }
     recvline[n++] = 0;
     fprintf(out, "%s\n", recvline);
   }

   exit(0);
}

