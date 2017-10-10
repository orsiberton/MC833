#include "my_socket_api.h"

int main (int argc, char **argv) {
   int listenfd, connfd, n;
   unsigned int clientaddr_len;
   struct sockaddr_in servaddr, clientaddr;
   char buf[MAXLINE], systembuf[MAXLINE], output[MAXLINE];
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

   // abre arquivo de log
   FILE *f = fopen("log_server.txt", "a+");
   if (f == NULL) {
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

	      // imprime no log os dados da conexao aberta
        PrintClientData((struct sockaddr_in *) &clientaddr, clientName, sizeof(clientName));
        FPrintClientData((struct sockaddr_in *) &clientaddr, clientName, sizeof(clientName), f);

        // le dados do cliente indefinidamente
        while ((n = read(connfd, buf, MAXLINE)) > 0) {

          printf("Executando comando (%s%c%d): %s", clientName, '/', ntohs(clientaddr.sin_port), buf);
          fprintf(f, "Executando comando (%s%c%d): %s", clientName, '/', ntohs(clientaddr.sin_port), buf);

          // Encerra a conexao
          if (isExit(buf)) {
	           isExiting = TRUE;
             PrintClientDataClose((struct sockaddr_in *) &clientaddr, clientName, sizeof(clientName));
             FPrintClientDataClose((struct sockaddr_in *) &clientaddr, clientName, sizeof(clientName), f);
             break;
          }

      	  // abre um terminal com o comando lido do cliente
          FILE *fp = popen(buf, "r");

          // le o resultado do comando executado
          while (fgets(systembuf, sizeof(systembuf) - 1, fp) != NULL) {
            strcat(output, systembuf);
          }

          // coloca o resultado no arquivo de log
      	  fprintf(f, "Retorno:\n%s\n", output);

      	  // fecha pseudoarquivo "terminal"
      	  pclose(fp);

          // envia ao cliente o resultado do comando
      	  write(connfd, output, strlen(output));
          memset(buf, 0, sizeof(buf));
          memset(output, 0, sizeof(output));
        }

        close(connfd);
      } else {
        // caso seja o processo pai
        close(connfd);
      }

    	if (isExiting) {
    		break;
    	}

   }

   // fecha arquivo de log
   fclose(f);

   return(0);
}
