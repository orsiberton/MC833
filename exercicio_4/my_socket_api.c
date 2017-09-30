#include "my_socket_api.h"

/*
  Função para criação de sockets
*/
int Socket(int family, int type, int flags) {
  int sockfd;
  if ((sockfd = socket(family, type, flags)) < 0) {
    perror("Error creating socket!");
    exit(1);
  }

  return(sockfd);
}

/*
  Função para abrir uma conexão
*/
void Connect(int socket, const struct sockaddr *sockaddr, socklen_t sockaddr_len) {
  if (connect(socket, sockaddr, sockaddr_len) < 0) {
     perror("Connect error");
     exit(1);
  }
}

/*
  Função para fazer o bind do socket
*/
void Bind(int socket, const struct sockaddr *sockaddr, socklen_t sockaddr_len) {
  if (bind(socket, sockaddr, sockaddr_len) == -1) {
     perror("Bind error");
     exit(1);
  }
}

/*
  Função para deixar o socket ouvindo conexões com um certo buffer
*/
void Listen(int socket, int queue_size) {
  if (listen(socket, queue_size) == -1) {
     perror("Listen error");
     exit(1);
  }
}

/*
  Função para aceitar conexões em um socket
*/
int Accept(int socket, struct sockaddr *sockaddr, socklen_t *sockaddr_len) {
  int sockfd;
  if ((sockfd = accept(socket, sockaddr, sockaddr_len)) == -1) {
    perror("Accept error");
    exit(1);
  }

  return(sockfd);
}

/*
  Função auxiliar para sair da conexão
*/
bool isExit(const char *message) {
  if (strncmp(message, "exit\n", strlen(message)) == 0) {
    return TRUE;
  }

  return FALSE;
}
