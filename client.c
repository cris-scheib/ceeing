#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define PORT 9000
#define SERVER_ADDR "127.0.0.1"
#define TAM 4096

int main(int argc, char *argv[])
{

	if(argc!=4)
	{
		printf("Numero de argumentos invalidos. Exemplo: ./cleint ARG1 ARG2 ARG3");
		exit(1);
	}
	   	
	int clientId = -1;
	struct sockaddr_in client;
	struct hostent *host;
	int response;

	char buffer_in[TAM];
	char buffer_out[TAM];

	clientId = socket(AF_INET, SOCK_STREAM, 0);
	if (clientId <= 0)
	{
		printf("Erro ao criar o socket\n");
		return -1;
	}
	else
	{
		printf("Cliente iniciou ...\n");
	}

	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	client.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	if (connect(clientId, (struct sockaddr *)&client, sizeof(client)))
	{
		printf("Erro ao conectar no endereco: %s e porta: %d\n", SERVER_ADDR, PORT);
		return -1;
	}
	sprintf(buffer_out,"ID: %s, sequencia: %s, valor: %s.",argv[1],argv[2],argv[3]);

	send(clientId, buffer_out, strlen(buffer_out), 0);

	response = recv(clientId, buffer_in, TAM, 0);
    	printf("Servidor respondeu: %s\n", buffer_in);

	close(clientId);

	return 0;
}
