#include "my_socket_api.h"

int main (int argc, char **argv) {
   int    listenfd, connfd, n;
   unsigned int peeraddr_len;
   struct sockaddr_in servaddr, peeraddr;
   char   buf[MAXLINE];

   // verifica se a porta foi passado por parametro
   if (argc != 2) {
      perror("Porta nao informada!");
      exit(1);
   }

   // cria um socket TCP
   listenfd = Socket(AF_INET, SOCK_STREAM, 0);

   // configura os parâmetros da conexão
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family      = AF_INET;
   servaddr.sin_addr.s_addr = INADDR_ANY;
   servaddr.sin_port        = htons(atoi(argv[1]));

   // faz o bind do socket TCP com o host:porta escolhidos
   Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

   // ativa a socket para começar a receber conexões
   Listen(listenfd, LISTENQ);

   // espera por conexões de clientes indefinidamente
   for ( ; ; ) {

    	// aceita as conexões
    	connfd = Accept(listenfd, (struct sockaddr *) NULL, NULL);

      // imprime dados do socket do cliente
      peeraddr_len = sizeof(struct sockaddr);
      if (getpeername(connfd, (struct sockaddr *) &peeraddr, &peeraddr_len) == -1) {
         perror("getpeername() failed");
         return -1;
      }
      printf("IP address do cliente: %s\n", inet_ntoa(peeraddr.sin_addr));
      printf("Porta do cliente: %d\n", (int) ntohs(peeraddr.sin_port));

      while ((n = read(connfd, buf, MAXLINE)) > 0) {

        printf("Executando comando: %s", buf);
        system(buf);

        // Encerra a conexao PARTE 2 DO TRABALHO
        //if (isExit(buf)) {
        //  break;
        //}

        write(connfd, buf, strlen(buf));
        memset(buf, 0, sizeof(buf));
      }

      close(connfd);
   }

   return(0);
}
