CC		= gcc
FLAGS = -Wall
APIS	= my_socket_api.o

all: $(APIS) servidor cliente

servidor: servidor.c
	$(CC) $(FLAGS) -o servidor $(APIS) servidor.c

cliente: cliente.c
	$(CC) $(FLAGS) -o cliente $(APIS) cliente.c

my_socket_api.o: my_socket_api.c
	$(CC) $(FLAGS) -c my_socket_api.c

clean:
	rm $(APIS) servidor cliente
