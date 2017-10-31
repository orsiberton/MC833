#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>

#define MAXLINE 4096
#define LISTENQ 5

/*
  Define um typo booleano
*/
typedef enum {
  FALSE = 0,
  TRUE = 1
} bool;

/*
  Funções que abstraem a interface de sockets
*/
int Socket(int family, int type, int flags);
void Connect(int socket, const struct sockaddr *sockaddr, socklen_t sockaddr_len);
void Bind(int socket, const struct sockaddr *sockaddr, socklen_t sockaddr_len);
void Listen(int socket, int queue_size);
int Accept(int socket, struct sockaddr *sockaddr, socklen_t *sockaddr_len);

/*
  Funções auxiliares
*/
bool isExit(const char *message);
pid_t Fork();
void PrintClientData(const struct sockaddr_in *sockaddr, char *clientName, int clientName_len);
void FPrintClientData(const struct sockaddr_in *sockaddr, char *clientName, int clientName_len, FILE *f);
void PrintClientDataClose(const struct sockaddr_in *sockaddr, char *clientName, int clientName_len);
void FPrintClientDataClose(const struct sockaddr_in *sockaddr, char *clientName, int clientName_len, FILE *f);
void PrintData(int socket, const struct sockaddr_in *sockaddr, char *localHost, char *localPort);
int Select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds, struct timeval *restrict timeout);
