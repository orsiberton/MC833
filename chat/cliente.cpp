#include "my_socket_api.h"

int main(int argc, char **argv) {
  int server_udp_socket_number, bytes_number, option_udp = 1;
  char data_received[MAXLINE], user_input[MAXLINE], nickname[100];
  struct sockaddr_in server_udp_socket;
  fd_set selectfd;

  // verifica se o host e a porta foram passados
  if (argc != 3) {
    perror("Host/Porta nao informados!/n");
    exit(1);
  }

  printf("\nMC833 - Chat\n");
  printf("Instruções do chat:\n");
  printf(" 1) Para exibir a lista de usuários online: list\n");
  printf(" 2) Para enviar uma mensagem: send <nickname> <message>\n");
  printf(" 3) Para transferir um arquivo: send-file <nickname> <file name>\n");
  printf("  3.1) Arquivos enviados para você serão salvos em uma pasta chama /chat-tmp-files\n");
  printf(" 4) Para se desconectar do chat: exit\n\n");

  printf("Entre com o seu nickname: \n");
  scanf("%s", nickname);

  printf("Bem-vindo %s!\n", nickname);

  // cria um socket UDP
  server_udp_socket_number = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  Setsockopt(server_udp_socket_number, SOL_SOCKET, SO_REUSEADDR, & option_udp, sizeof(int));

  // configura os parâmetros da conexão
  bzero( & server_udp_socket, sizeof(server_udp_socket));
  server_udp_socket.sin_family = AF_INET;
  server_udp_socket.sin_addr.s_addr = inet_addr(argv[1]);
  server_udp_socket.sin_port = htons(atoi(argv[2]));

  // abre a conexão com o servidor
  Connect(server_udp_socket_number, (struct sockaddr * ) &server_udp_socket, sizeof(server_udp_socket));

  FD_ZERO( & selectfd);

  // le entrada do cliente
  for (;;) {
    // faz o set no stdin e no socket
    FD_SET(fileno(stdin), &selectfd);
    FD_SET(server_udp_socket_number, &selectfd);

    // faz o select em selectfd
    Select(FD_SETSIZE, &selectfd, NULL, NULL, NULL);

    // verifica se o stdin foi setado
    if (FD_ISSET(fileno(stdin), &selectfd)) {
      // le do stdin
      bytes_number = read(fileno(stdin), user_input, MAXLINE);

      // caso algo tenha sido lido
      if (bytes_number > 0) {
        Send(server_udp_socket_number, user_input, bytes_number, 0);

        // termina execução do cliente caso o cliente queira
        if (isExit(user_input)) {
          break;
        }

        memset(user_input, 0, sizeof(user_input));
      }
    }

    // verifica se o socket foi setado
    if (FD_ISSET(server_udp_socket_number, &selectfd)) {
      bytes_number = Recv(server_udp_socket_number, data_received, MAXLINE, 0);
      write(fileno(stdout), data_received, bytes_number);
    }
  }

  return 0;
}
