#include "my_socket_api.h"
#include <pthread.h>

static const char EXIT_COMMAND[] = "exit";

int sockfd;
struct sockaddr_in servaddr;

void *receiveMessages(){
   char recvline[MAXLINE];

   while(TRUE){
      int n = recvfrom(sockfd, recvline, MAXLINE, 0, &servaddr, sizeof(servaddr));

      if(n > 0){
         if(strstr(recvline, SERVER_SEND) == 0){
            // TODO fazer parse do recvline separando SERVER_SEND, remetente e mensagem
            printf("%s\n", recvline);

            while (sendto(sockfd, SERVER_SEND_OK, sizeof(SERVER_SEND_OK), 0, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1){
               // try to send until successful
            }
         } else if(strstr(recvline, CLIENT_SEND_DENIED) == 0){
            printf("--Mensagem nao enviada; tente novamente.--\n");
         }
         
      }

      sleep(5);
   }
}

// realiza uma iteracao do chat
// retorna 0 caso o chat deva ser encerrado
// retorna 1 caso o chat deva continuar
int chat(){
   char input[MAXLINE];

   fgets(input, MAXLINE, stdin);

   if(strstr(input, EXIT_COMMAND) == 0){
      return 0;
   }

   // TODO montar string a ser enviada, trocando CLIENT_SEND abaixo por uma string composta
   // por CLIENT_SEND, destinatario e mensagem

   while (sendto(sockfd, CLIENT_SEND, sizeof(CLIENT_SEND), 0, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1){
      // try to send until successful
   }

   return 1;
}

// realiza processo de saida do chat
void exitChat(){
   while (sendto(sockfd, CLIENT_LEAVING, sizeof(CLIENT_LEAVING), 0, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1){
      // try to exit chat until successful
   }

   exitChat(sockfd, servaddr); // repeat until success
}

int main(int argc, char **argv) {
   char   recvline[MAXLINE], input[MAXLINE];
   pthread_t receiver;

   // verifica se o host foi passado
   if (argc != 2) {
      perror("Host do servidor nao informado!");
      exit(1);
   }

   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(CHAT_PORT);
   servaddr.sin_addr.s_addr = inet_addr(argv[1]);

   // cria um socket UDP
   sockfd = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

   Bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr) );

   // socket reutiliza o endereço IP
   //Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

   Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

   while (sendto(sockfd, CLIENT_JOIN, sizeof(CLIENT_JOIN), 0, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1){
      // try to join chat until successful
   }

   // client joined

   // TODO pegar a lista de clientes disponiveis para bater papo
   // TODO pedir para usuario selecionar cliente e informar servidor

   pthread_create(&receiver, NULL, receiveMessages, NULL);
   pthread_join(receiver, NULL);

   while(TRUE){
      if( !chat(sockfd, servaddr) ){
         pthread_cancel(receiver);
         exitChat(sockfd, servaddr);
         break;
      }
   }

   exit(0);
}
