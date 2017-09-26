#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

#define LISTENQ 10
#define MAXDATASIZE 100

int main (int argc, char **argv) {
   int    listenfd, connfd;
   unsigned int peeraddr_len;
   struct sockaddr_in servaddr, peeraddr;
   char   buf[MAXDATASIZE];
   time_t ticks;

   // verifica se a porta foi passado por parametro
   if (argc != 2) {
      perror("Porta nao informada!");
      exit(1);
   }

   // cria um socket TCP
   if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("socket");
      exit(1);
   }

   // configura os parâmetros da conexão
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family      = AF_INET;
   servaddr.sin_addr.s_addr = INADDR_ANY;
   servaddr.sin_port        = htons(atoi(argv[1]));

   // faz o bind do socket TCP com o host:porta escolhidos
   if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
      perror("bind");
      exit(1);
   }

   // ativa a socket para começar a receber conexões
   if (listen(listenfd, LISTENQ) == -1) {
      perror("listen");
      exit(1);
   }

   // espera por conexões de clientes indefinidamente
   for ( ; ; ) {

      // aceita as conexões
      if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1 ) {
         perror("accept");
         exit(1);
      }

      ticks = time(NULL);
      snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
      // escreve no socket para que o cliente receba a mensagem
      write(connfd, buf, strlen(buf));

      printf("Recebi uma conexão\n");

      printf("Dormindo\n");
      sleep(5);
      printf("Acordando\n");

      // imprime dados do socket do cliente
      peeraddr_len = sizeof(struct sockaddr);
      if (getpeername(connfd, (struct sockaddr *) &peeraddr, &peeraddr_len) == -1) {
         perror("getpeername() failed");
         return -1;
      }
      printf("IP address do cliente: %s\n", inet_ntoa(peeraddr.sin_addr));
      printf("Porta do cliente: %d\n", (int) ntohs(peeraddr.sin_port));

      close(connfd);
   }

   return(0);
}
