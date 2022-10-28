#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <Winsock2.h>
#include <unistd.h>

#define PORT 9000
#define BUFFER_TAM 4096

typedef struct
{
	int serverId;
	struct sockaddr server;
	int addr_len;
} connection_t;

void *process(void *ptr)
{
	char buffer[BUFFER_TAM];
	int len;
	connection_t *conn;
	long addr = 0;

	if (!ptr)
	{
		pthread_exit(0);
	}
	conn = (connection_t *)ptr;

	strcpy(buffer, "Informacao recebida!\n\0");

	if (send(conn->serverId, buffer, strlen(buffer), 0))
	{

		memset(buffer, 0x0, BUFFER_TAM);

		int mensagem_tam;
		if ((mensagem_tam = recv(conn->serverId, buffer, BUFFER_TAM, 0)) > 0)
		{
			buffer[mensagem_tam - 1] = '\0';
			printf("Recebido: %s\n", buffer);
		}
	}else{
		printf("Erro ao enviar a mensagem\n");
	}
	close(conn->serverId);
	free(conn);
	pthread_exit(0);
}

int main()
{
	int serverId = -1;
	struct sockaddr_in server;
	connection_t *connection;
	pthread_t thread;

	serverId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverId <= 0)
	{
		printf("Erro ao criar o socket\n");
		return -1;
	}
	else
	{
		printf("Servidor iniciou ...\n");
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	if (bind(serverId, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0)
	{
		printf("Erro ao criar o socket na porta: %d\n", PORT);
		return -1;
	}

	if (listen(serverId, 1) < 0)
	{
		printf("Erro ao ouvir a porta: %d\n", PORT);
		return -1;
	}

	printf("Escutando a porta %d\n", PORT);

	while (1)
	{
		connection = (connection_t *)malloc(sizeof(connection_t));
		connection->serverId = accept(serverId, &connection->server, &connection->addr_len);
		if (connection->serverId <= 0)
		{
			free(connection);
			printf("Conexao liberada\n");
		}
		else
		{
			pthread_create(&thread, 0, process, (void *)connection);
			pthread_detach(thread);
		}
	}

	return 0;
}
