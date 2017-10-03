#include "my_socket_api.h"

int main (int argc, char **argv) {
   int listenfd, connfd, n;
   unsigned int clientaddr_len;
   struct sockaddr_in servaddr, clientaddr;
   char buf[MAXLINE], systembuf[MAXLINE];
   char clientName[INET_ADDRSTRLEN];
   pid_t pid;
   bool isExiting = FALSE;

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

	FILE *f = fopen("log_server.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

   // espera por conexões de clientes indefinidamente
   for ( ; ; ) {

    	// aceita as conexões
      clientaddr_len = sizeof(clientaddr);
      connfd = Accept(listenfd, (struct sockaddr *) &clientaddr, &clientaddr_len);

      // cria um processo filho
      pid = Fork();

      // caso seja o processo filho
      if (pid == 0) {
        // fecha a conexão de escuta para esse processo filho
        close(listenfd);

        FPrintClientData((struct sockaddr_in *) &clientaddr, clientName, sizeof(clientName), f);

        // le dados do cliente indefinidamente
        while ((n = read(connfd, buf, MAXLINE)) > 0) {

          fprintf(f, "Executando comando (%s%c%d): %s", clientName, '/', ntohs(clientaddr.sin_port), buf);
          //system(buf);
          
          FILE *fp = popen(buf, "r");
		  fscanf(fp, "%s", systembuf);
		  fprintf(f, "Retorno: %s\n", systembuf);
		  write(connfd, systembuf, strlen(systembuf));
  		  pclose(fp);

          // Encerra a conexao PARTE 2 DO TRABALHO
          if (isExit(buf)) {
		isExiting = TRUE;
		//printf("Cliente (%s%c%d) fazendo logout!\n", clientName, '/', ntohs(clientaddr.sin_port));

          }

          // retorna o que foi enviado pelo cliente para o cliente
          //write(connfd, buf, strlen(buf));
          memset(buf, 0, sizeof(buf));
        }
        
        FPrintClientDataClose((struct sockaddr_in *) &clientaddr, clientName, sizeof(clientName), f);
        close(connfd);
      } else {
        // caso seja o processo pai
        close(connfd);
      }

	if(isExiting){
		break;
	}

   }
   
	fclose(f);

   return(0);
}
