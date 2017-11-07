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
  Constantes
*/
static const char CLIENT_JOIN[] = "join";
static const char CLIENT_JOIN_OK[] = "join_ok";
static const char CLIENT_JOIN_DENIED[] = "join_denied";

static const char CLIENT_SEND[] = "client_send";
static const char CLIENT_SEND_OK[] = "client_send_ok";
static const char CLIENT_SEND_DENIED[] = "client_send_denied";

static const char SERVER_SEND[] = "server_send";
static const char SERVER_SEND_OK[] = "server_send_ok";

static const char CLIENT_LEAVING[] = "leaving";
static const char CLIENT_LEAVING_OK[] = "leaving_ok";

static const char SEPARATOR[] = "*&$#@!";

static const unsigned int CHAT_PORT = 25713;

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
void PrintData(int socket, const struct sockaddr_in *sockaddr, char *localHost, char *localPort);
