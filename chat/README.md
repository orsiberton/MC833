# MC833 - Chat

### Como compilar
Para compilar o código do servidor e do cliente basta executar o comando:
```sh
$ make
```

### Como executar o servidor
Para executar o servidor basta executar o comando:
```sh
$ ./servidor
```
O servidor será iniciado na porta `25713`.

### Como executar o cliente
Para executar o cliente basta executar o comando:
```sh
$ ./cliente <host do servidor>
```
Assim que o cliente for executado, um texto explicativo será exibido para o cliente:

MC833 - Chat
Instruções do chat:
 1) Para exibir a lista de usuários online: list
 2) Para enviar uma mensagem: send <nickname> <message>
 3) Para transferir um arquivo: transfer-file <nickname> <file name>
  3.1) Arquivos enviados para você serão salvos em uma pasta chama /chat-tmp-files
 4) Para se desconectar do chat: exit

 Dessa forma o cliente não ficará perdido e saberá o que fazer.
