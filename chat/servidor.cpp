#include "my_socket_api.h"

int main(int argc, char **argv) {
  int option_udp = 1;
  int client_udp_socket_number;
  unsigned int client_udp_socket_size;
  struct sockaddr_in client_udp_socket, server_udp_socket;
  char data_received[MAXLINE];
  char client_name[INET_ADDRSTRLEN];
  vector<Client> client_list;
  FILE *log_file;

  // cria um socket UDP
  client_udp_socket_number = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  Setsockopt(client_udp_socket_number, SOL_SOCKET, SO_REUSEADDR, &option_udp, sizeof(int));

  memset( & server_udp_socket, 0, sizeof(server_udp_socket));
  server_udp_socket.sin_addr.s_addr = INADDR_ANY;
  server_udp_socket.sin_port = htons(CHAT_PORT);
  server_udp_socket.sin_family = AF_INET;

  // realiza o bind dos sockets cliente e servidor
  Bind(client_udp_socket_number, (struct sockaddr * ) &server_udp_socket, sizeof(server_udp_socket));

  // espera por mensagens de clientes
  for (;;) {
    // recebe a mensagem dos clientes
    client_udp_socket_size = sizeof(client_udp_socket);
    Recvfrom(client_udp_socket_number, data_received, sizeof(data_received), 0, (struct sockaddr * ) &client_udp_socket, &client_udp_socket_size);

    inet_ntop(AF_INET, &client_udp_socket.sin_addr.s_addr, client_name, sizeof(client_name));

    // log as mensagens trafegadas pelos clientes
    log_file = fopen("log_server.txt", "a+");
    fprintf(log_file, "Cliente(%s/%d):\n\t%s\n", client_name, ntohs(client_udp_socket.sin_port), data_received);
    fclose(log_file);

    // handler para conexão de novo cliente
    if (startsWith("connect-client", data_received)) {
      char nickname_temp[100];
      sscanf(data_received, "connect-client %s", nickname_temp);

      Client client;
      client.nickname = nickname_temp;
      client.socket_number = client_udp_socket_number;
      client.host = client_name;
      client.port_number = ntohs(client_udp_socket.sin_port);
      client.client_udp_socket = client_udp_socket;
      // adiciona cliente na lista
      client_list.push_back(client);

      // log a conexão do cliente
      log_file = fopen("log_server.txt", "a+");
      fprintf(log_file, "Cliente se conectou:\n\tNickname: %s\n\tEndereço IP: %s\n\tPorta: %d\n", nickname_temp, client_name, ntohs(client_udp_socket.sin_port));
      fclose(log_file);

    } else if (startsWith("list", data_received)) {
      // handler para listagem de clientes conectados
      string message = "Lista de clientes conectados:\n";
      Sendto(client_udp_socket_number, message.c_str(), strlen(message.c_str()), 0, (struct sockaddr * ) &client_udp_socket, client_udp_socket_size);
      for(vector<Client>::iterator it = client_list.begin(); it != client_list.end(); ++it) {
        Client& client = *it;
        message = client.nickname.c_str();
        message += '\n';
        Sendto(client_udp_socket_number, message.c_str(), strlen(message.c_str()), 0, (struct sockaddr * ) &client_udp_socket, client_udp_socket_size);
      }
    } else if (startsWith("send", data_received)) {
      // handler para mandar mensagem de um cliente para o outro
      char receiver_nickname[100];
      char client_message[MAXLINE];
      string current_client_nickname;
      sscanf(data_received, "send %s %[^\t]", receiver_nickname, client_message);

      // faz a iteração para descobrir o nickname de quem está mandando a mensagem
      for(vector<Client>::iterator it = client_list.begin(); it != client_list.end(); ++it) {
        Client& client = *it;
        if (strcmp(client.host.c_str(), client_name) == 0 && client.port_number == ntohs(client_udp_socket.sin_port)) {
          current_client_nickname = client.nickname;
          break;
        }
      }

      // monta a mensagem para o cliente destino
      string message = current_client_nickname;
      message += ": ";
      message += client_message;

      // tenta enviar a mensagem para o cliente destino
      bool message_sent = false;
      for(vector<Client>::iterator it = client_list.begin(); it != client_list.end(); ++it) {
        Client& client = *it;
        if (strcmp(client.nickname.c_str(), receiver_nickname) == 0) {
          Sendto(client.socket_number, message.c_str(), strlen(message.c_str()), 0, (struct sockaddr * ) &client.client_udp_socket, client_udp_socket_size);
          message_sent = true;
        }
      }

      if (!message_sent) {
        message = "Falha ao enviar a mensagem: Cliente não encontrado!\n";
        Sendto(client_udp_socket_number, message.c_str(), strlen(message.c_str()), 0, (struct sockaddr * ) &client_udp_socket, client_udp_socket_size);
      }
    } else if (startsWith("transfer-file", data_received)) {
      // handler para enviar IP e porta para o cliente, assim o cliente abrirá a conexão TCP
      char receiver_nickname[100];
      char file_name[MAXLINE];
      sscanf(data_received, "transfer-file %s %[^\t\n]", receiver_nickname, file_name);

      string message = "open-connection-to-transfer-file ";
      message += file_name;

      // tenta enviar a mensagem para o cliente destino
      bool message_sent = false;
      for(vector<Client>::iterator it = client_list.begin(); it != client_list.end(); ++it) {
        Client& client = *it;
        if (strcmp(client.nickname.c_str(), receiver_nickname) == 0) {
          // notifica o cliente destino para abrir a conexão
          Sendto(client.socket_number, message.c_str(), strlen(message.c_str()), 0, (struct sockaddr * ) &client.client_udp_socket, client_udp_socket_size);

          // notifica o cliente o host do cliente destino
          message = client.host;
          Sendto(client_udp_socket_number, message.c_str(), strlen(message.c_str()), 0, (struct sockaddr * ) &client_udp_socket, client_udp_socket_size);
          message_sent = true;
        }
      }

      if (!message_sent) {
        message = "Falha ao enviar a mensagem: Cliente não encontrado!\n";
        Sendto(client_udp_socket_number, message.c_str(), strlen(message.c_str()), 0, (struct sockaddr * ) &client_udp_socket, client_udp_socket_size);
      }
    } else if (startsWith("exit", data_received)) {
      // handler para remover um cliente da lista
      int index = 0;
      bool client_found = false;
      string nickname;
      // faz a iteração sobre os clientes para saber qual será apagado da lista
      for(vector<Client>::iterator it = client_list.begin(); it != client_list.end(); ++it) {
        Client& client = *it;
        if (strcmp(client.host.c_str(), client_name) == 0 && client.port_number == ntohs(client_udp_socket.sin_port)) {
          client_found = true;
          nickname = client.nickname;
          break;
        }

        index++;
      }

      // se o cliente foi encontrado, ele é removido
      if (client_found) {
        client_list.erase(client_list.begin() + index);
      }

      // log a saída do cliente
      log_file = fopen("log_server.txt", "a+");
      fprintf(log_file, "Cliente se desconectou:\n\tNickname: %s\n\tEndereço IP: %s\n\tPorta: %d\n", nickname.c_str(), client_name, ntohs(client_udp_socket.sin_port));
      fclose(log_file);
    } else {
      // Comando inválido
      string message = "Comando inválido!\n";
      Sendto(client_udp_socket_number, message.c_str(), strlen(message.c_str()), 0, (struct sockaddr * ) &client_udp_socket, client_udp_socket_size);
    }

    memset(data_received, 0, sizeof(data_received));
  }

  return (0);
}
