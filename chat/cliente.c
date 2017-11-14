#include "my_socket_api.h"

int main(int argc, char **argv) {
  int sockfd, n, optval = 1;
  char recvline[MAXLINE], input[MAXLINE];
  struct sockaddr_in servaddr;
  struct hostent * host_address;
  fd_set selectfd;

  // verifica se o host foi passado
  if (argc != 2) {
    perror("Host do servidor nao informado!");
    exit(1);
  }

  // cria um socket UDP
  sockfd = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  // socket reutiliza o endereço IP
  Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

  host_address = gethostbyname(argv[1]);
  if (!host_address) {
    perror("Host nao encontrado");
    exit(1);
  }

  // Configura a conexao com o servidor
  memset(&servaddr, 0, sizeof(servaddr));
  bcopy(host_address->h_addr, (char *) &servaddr.sin_addr, host_address->h_length);
  servaddr.sin_port = htons(CHAT_PORT);
  servaddr.sin_family = AF_INET;

  // Realiza a 'conexao' UDP com o servidor
  Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

  FD_ZERO(&selectfd);

  // TODO mandar comando ao servidor pedindo clientes para se conectar?

  // le entrada do cliente/recebe dados do servidor
  for ( ; ; ) {
    // faz o set no stdin e no socket
    FD_SET(fileno(stdin), &selectfd);
    FD_SET(sockfd, &selectfd);

    // faz o select em selectfd
    Select(FD_SETSIZE, &selectfd, NULL, NULL, NULL);

    // verifica se o stdin foi setado
    if (FD_ISSET(fileno(stdin), &selectfd)) {
      // le do TECLADO do cliente
      n = read(fileno(stdin), recvline, MAXLINE);

      // caso algo tenha sido enviado
      if (n > 0) {
        write(sockfd, recvline, n);
      }
    }

    // verifica se o socket foi setado
    if (FD_ISSET(sockfd, &selectfd)) {
      // le do SERVIDOR
      n = read(sockfd, input, MAXLINE);
      write(fileno(stdout), input, n);
    }

    printf("entrada cliente: %s\n", recvline);
  }

  exit(0);
}
