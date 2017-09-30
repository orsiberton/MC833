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

/*
  Função auxiliar que abre outro processo
*/
pid_t Fork() {
  pid_t pid;
  if ((pid = fork()) < 0) {
    perror("Fork error");
    exit(1);
  }

  return pid;
}

/*
  Função auxiliar que imprime os dados do socket cliente
*/
void PrintClientData(const struct sockaddr_in *sockaddr, char *clientName, int clientName_len) {
  if (inet_ntop(AF_INET, &sockaddr->sin_addr.s_addr, clientName, clientName_len) != NULL) {
     printf("Endereco IP do cliente: %s\n", clientName);
     printf("Porta do cliente: %d\n", ntohs(sockaddr->sin_port));
  } else {
     printf("Erro ao imprimir dados do cliente!\n");
  }
}

/*
  Função auxiliar que imprime os dados do socket do lado do cliente
*/
void PrintData(int socket, const struct sockaddr_in *sockaddr, char *localHost, char *localPort) {
  unsigned int sockaddr_len = sizeof(struct sockaddr);
  if (getsockname(socket, (struct sockaddr *) sockaddr, &sockaddr_len) == -1) {
    perror("getsockname() failed");
    exit(1);
  }
  printf("Endereco IP remoto do socket: %s\n", inet_ntoa(sockaddr->sin_addr));
  printf("Porta remota do socket: %d\n", (int) ntohs(sockaddr->sin_port));
  printf("Endereco IP local do socket: %s\n", localHost);
  printf("Porta local do socket: %s\n", localPort);
}
