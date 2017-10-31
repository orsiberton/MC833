#include "my_socket_api.h"

int main (int argc, char **argv) {
   int listenfd, connfd, activity, nread;
   unsigned int clientaddr_len;
   struct sockaddr_in servaddr, clientaddr;
   char buf[MAXLINE];
   fd_set selectfd;
	 fd_set selectfd_aux;

   // verifica se o nome do arquivo foi passado por parametro
   if (argc != 2) {
      perror("Porta nao informado!");
      exit(0);
   }

   // cria um socket TCP
   listenfd = Socket(AF_INET, SOCK_STREAM, 0);

   // cria o socket TCP
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(atoi(argv[1]));

   // faz o bind do socket TCP com o host:porta escolhidos
   Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

   // ativa a socket para começar a receber conexões
   Listen(listenfd, LISTENQ);

   FD_ZERO(&selectfd);
   FD_SET(listenfd, &selectfd);
   // espera por conexões de clientes indefinidamente
   for ( ; ; ) {
     selectfd_aux = selectfd;
     activity = Select(FD_SETSIZE, &selectfd_aux, NULL, NULL, NULL);

     // se houve uma conexao a flag é setada e a conexao eh aceita
     if (FD_ISSET(listenfd, &selectfd_aux)) {
       activity--;
       clientaddr_len = sizeof(clientaddr);
       connfd = Accept(listenfd, (struct sockaddr *) &clientaddr, &clientaddr_len);
       FD_SET(connfd, &selectfd);
     }

     // se a flag estiver setada
     if (FD_ISSET(connfd, &selectfd_aux)) {
       activity--;

       // le a linha e manda de volta para o cliente
       nread = read(connfd, buf, MAXLINE);
       write(connfd, buf, nread);

       // caso não tenha nada para ler, limpa
       if (nread == 0) {
         close(connfd);
         FD_CLR(connfd, &selectfd);
       }
     }
   }

   // fecha o socket de listen
   close(listenfd);

   return(0);
}
