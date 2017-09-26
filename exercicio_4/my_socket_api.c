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
