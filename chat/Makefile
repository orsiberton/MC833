CC	= g++
FLAGS = -Wall
APIS	= my_socket_api.o

all: $(APIS) servidor cliente

servidor: servidor.cpp
	$(CC) $(FLAGS) -o servidor $(APIS) servidor.cpp

cliente: cliente.cpp
	$(CC) $(FLAGS) -o cliente $(APIS) cliente.cpp

my_socket_api.o: my_socket_api.cpp
	$(CC) $(FLAGS) -c my_socket_api.cpp

clean:
	rm $(APIS) servidor cliente
