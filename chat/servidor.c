#include "my_socket_api.h"

int main (int argc, char **argv) {
   int listenfd, n;
   unsigned int clientaddr_len;
   struct sockaddr clientaddr, clients[MAX_CLIENTS];
   struct sockaddr_in serveraddr;
   char buf[MAXLINE];

   serveraddr.sin_family = AF_INET;
   serveraddr.sin_port = htons(CHAT_PORT);
   serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

   // cria um socket UDP
   listenfd = Socket(AF_INET, SOCK_DGRAM, 0);

   Bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr) );

   for(;;){
      n = recvfrom(listenfd, buf, MAXLINE, 0, &clientaddr, &clientaddr_len);

      if(n >= 0){
        if(strstr(buf, CLIENT_JOIN) == 0){

        } else if (strstr(buf, CLIENT_SEND) == 0){

        } else if (strstr(buf, CLIENT_LEAVING) == 0){
          while (sendto(listenfd, CLIENT_LEAVING_OK, sizeof(CLIENT_LEAVING_OK), 0, (struct sockaddr*) &clientaddr, clientaddr_len) == -1){
              // try to send until successful
          }
        }
      }
   }

   return(0);
}
