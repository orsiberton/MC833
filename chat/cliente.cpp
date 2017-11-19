#include "my_socket_api.h"

int main(int argc, char **argv) {
  int transfer_socket_listen_number, transfer_socket_number;
  int server_udp_socket_number, bytes_number, option_udp = 1;
  char data_received[MAXLINE], user_input[MAXLINE], nickname[100];
  struct sockaddr_in server_udp_socket;
  struct sockaddr_in transfer_file_socket;
  fd_set selectfd;
  struct hostent *server_host;

  // verifica se o host e a porta foram passados
  if (argc != 2) {
    perror("Host nao informado!/n");
    exit(1);
  }

  // comando para criar a pasta que ficarão os arquivos transferidos
  system("mkdir -p chat-tmp-files");

  printf("\nMC833 - Chat\n");
  printf("Instruções do chat:\n");
  printf(" 1) Para exibir a lista de usuários online: list\n");
  printf(" 2) Para enviar uma mensagem: send <nickname> <message>\n");
  printf(" 3) Para transferir um arquivo: transfer-file <nickname> <file name>\n");
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
  server_udp_socket.sin_port = htons(CHAT_PORT);

  // Faz o set fo host por IP ou nome
  if (isValidIP(argv[1])) {
    server_udp_socket.sin_addr.s_addr = inet_addr(argv[1]);
  } else {
    server_host = gethostbyname(argv[1]);
    bcopy((char *) server_host->h_addr, (char *) &server_udp_socket.sin_addr.s_addr, server_host->h_length);
  }

  // abre a conexão com o servidor
  Connect(server_udp_socket_number, (struct sockaddr * ) &server_udp_socket, sizeof(server_udp_socket));

  // cliente pede para se registrar
  string register_command = "connect-client ";
  register_command += nickname;
  Send(server_udp_socket_number, register_command.c_str(), strlen(register_command.c_str()), 0);

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

        // handler para enviar o arquivo diretamente para o outro cliente
        if (startsWith("transfer-file", user_input)) {
          char other_client_address[MAXLINE];
          char file_name[MAXLINE];
          char receiver_nickname[MAXLINE];
          sscanf(user_input, "transfer-file %s %[^\t\n]", receiver_nickname, file_name);
          Recv(server_udp_socket_number, other_client_address, MAXLINE, 0);

          printf("Transferindo arquivo %s para %s\n", file_name, receiver_nickname);

          // cria um socket TCP
          transfer_socket_number = Socket(AF_INET, SOCK_STREAM, 0);

          // configura os parâmetros da conexão
          bzero(&transfer_file_socket, sizeof(transfer_file_socket));
          transfer_file_socket.sin_family = AF_INET;
          transfer_file_socket.sin_addr.s_addr = inet_addr(other_client_address);
          transfer_file_socket.sin_port = htons(TRANSFER_FILE_PORT);

          if (inet_pton(AF_INET, other_client_address, &transfer_file_socket.sin_addr) <= 0) {
             perror("inet_pton error");
             exit(1);
          }

          // abre a conexão com o outro cliente
          Connect(transfer_socket_number, (struct sockaddr *) &transfer_file_socket, sizeof(transfer_file_socket));

          char file_content[MAXLINE];
          FILE *file;
          file = fopen(file_name, "r");

          // escreve no socket até terminar
          while (fread(file_content, 1, sizeof(file_content), file) > 0) {
            write(transfer_socket_number, file_content, MAXLINE);
          }

          // manda um comando avisando que o arquivo acabou
          write(transfer_socket_number, "end-of-transfer", MAXLINE);

          printf("Transferencia completa!\n");
          close(transfer_socket_number);
          fclose(file);
        }

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

      // handler para abrir o cliente para conexões TCP para trocar arquivos
      if (startsWith("open-connection-to-transfer-file", data_received)) {
        char file_name[MAXLINE];
        sscanf(data_received, "open-connection-to-transfer-file %[^\t\n]", file_name);
        string file_path = "chat-tmp-files/";
        file_path += file_name;

        // cria um socket TCP
        transfer_socket_listen_number = Socket(AF_INET, SOCK_STREAM, 0);

        // configura os parâmetros da conexão
        bzero(&transfer_file_socket, sizeof(transfer_file_socket));
        transfer_file_socket.sin_family      = AF_INET;
        transfer_file_socket.sin_addr.s_addr = INADDR_ANY;
        transfer_file_socket.sin_port        = htons(TRANSFER_FILE_PORT);

        // faz o bind do socket TCP
        Bind(transfer_socket_listen_number, (struct sockaddr *) &transfer_file_socket, sizeof(transfer_file_socket));

        // ativa a socket para começar a receber conexões
        Listen(transfer_socket_listen_number, LISTENQ);

        transfer_socket_number = Accept(transfer_socket_listen_number, (struct sockaddr *) NULL, NULL);
        char file_content[MAXLINE];

        printf("Recebendo arquivo %s\n", file_name);

        FILE *file;
        file = fopen(file_path.c_str(), "w");

        // le o arquivo do socket e salva
        while (1) {
          read(transfer_socket_number, file_content, MAXLINE);

          if (strcmp(file_content, "end-of-transfer") != 0) {
            fprintf(file, "%s", file_content);
          } else {
            break;
          }
        }

        // fecha os sockets e o arquivo
        close(transfer_socket_number);
        close(transfer_socket_listen_number);
        fclose(file);

        printf("Arquivo recebido com sucesso!\n");
      } else {
        write(fileno(stdout), data_received, bytes_number);
      }
    }
  }

  return 0;
}
