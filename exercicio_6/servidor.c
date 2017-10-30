#include "my_socket_api.h"

int main (int argc, char **argv) {
   int listenfd, n, port, new_socket, activity;
   unsigned int clientaddr_len, maxfdp1;
   struct sockaddr_in servaddr, clientaddr;
   char buf[MAXLINE];
   fd_set rset;

   // verifica se o nome do arquivo foi passado por parametro
   if (argc != 3) {
      perror("Porta/Log nao informado!");
      exit(0);
   }

   port = atoi(argv[1]);

   // cria um socket TCP
   listenfd = Socket(AF_INET, SOCK_STREAM, 0);

   //type of socket created
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

   // faz o bind do socket TCP com o host:porta escolhidos
   Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

   // ativa a socket para começar a receber conexões
   Listen(listenfd, LISTENQ);

   FD_ZERO(&rset);

   // espera por conexões de clientes indefinidamente
   for ( ; ; ) {
      FD_SET(listenfd, &rset);
      maxfdp1 = listenfd;
      activity = select(maxfdp1 + 1, &rset , NULL , NULL , NULL);

      if ((activity < 0) && (errno!=EINTR)) {
          perror("select error");
          exit(1);
      }

      if ((new_socket = accept(listenfd, (struct sockaddr *)&clientaddr, (socklen_t*)&clientaddr_len))<0){
	       perror("accept error");
	       exit(1);
      }

    	while (FD_ISSET(listenfd, &rset)) {
    		if ((n = read(new_socket, buf, MAXLINE)) < 0) {
    		    perror("read error");
    		    exit(1);
        }

    		if (send(new_socket, buf, strlen(buf), 0) != strlen(buf) ) {
    	     perror("send error");
           exit(1);
    		}
    	}
   }

   // fecha arquivo de log
   close(listenfd);

   return(0);
}
