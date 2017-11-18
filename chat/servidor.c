#include "my_socket_api.h"

int main(int argc, char **argv) {
  int bytes_received;
  int option_udp = 1;
  int client_udp_socket_number;
  unsigned int client_udp_socket_size;
  struct sockaddr_in client_udp_socket, server_udp_socket;
  char data_received[MAXLINE];

  // verifica se a porta foi passado por parametro
  if (argc != 2) {
    perror("Porta nao informada!");
    exit(1);
  }

  // cria um socket UDP
  client_udp_socket_number = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  Setsockopt(client_udp_socket_number, SOL_SOCKET, SO_REUSEADDR, &option_udp, sizeof(int));

  memset( & server_udp_socket, 0, sizeof(server_udp_socket));
  server_udp_socket.sin_addr.s_addr = INADDR_ANY;
  server_udp_socket.sin_port = htons(atoi(argv[1]));
  server_udp_socket.sin_family = AF_INET;

  // realiza o bind dos sockets cliente e servidor
  Bind(client_udp_socket_number, (struct sockaddr * ) &server_udp_socket, sizeof(server_udp_socket));

  // espera por mensagens de clientes
  for (;;) {
    /* Recebe mensagem UPD: */
    client_udp_socket_size = sizeof(client_udp_socket);
    bytes_received = Recvfrom(client_udp_socket_number, data_received, sizeof(data_received), 0, (struct sockaddr * ) &client_udp_socket, &client_udp_socket_size);

    /* Envia o eco: */
    Sendto(client_udp_socket_number, data_received, bytes_received, 0, (struct sockaddr * ) &client_udp_socket, client_udp_socket_size);
    memset(data_received, 0, sizeof(data_received));
  }

  return (0);
}
