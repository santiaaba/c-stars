// implementacion de servidor UDP
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "prueba_buffer.h"

#define PORT 8080
#define MAXLINE 1024

typedef struct
{
    uint32_t frame;
    uint8_t type;
    uint16_t size; /* Cantidad de elementos del body en bytes */
    uint8_t aux;
} data_header_t;

typedef struct
{
    uint16_t entity_class;
    int16_t pos_x;
    int16_t pos_y;
    uint8_t sprite;
    uint8_t frame;
} data_render_t;

typedef struct
{
    data_header_t header;
    data_render_t body[MAX_DATA_BODY];
} data_t;

void data_to_buffer(data_t *data, char **buffer, int *size)
{
    /* Convierte la estructura data en buffer. Evitamos
		tener que andar redimencionando continuamente el
	   buffer. Asi que size debe ser un reflejo fiel del
		tamaño del buffer */
    int j = 0;

    data->body->entity_class = 20;
    data->body->pos_x = 20;
    data->body->pos_y = 20;
    data->body->sprite = 30;
    data->body->frame = 32;

    data->header.frame = 20;
    data->header.size = 1;
    data->header.type = 3;
    data->header.aux = 1;

    /* Encabezado */
    if (*size < DATA_HEAD_SIZE + (data->header.size)) // size expresado en bytes
    {
        *size = DATA_HEAD_SIZE + (data->header.size * 8);
        *buffer = (char *)realloc(*buffer, *size);
    }
    printf("Buffer: %s\n", buffer);
    *buffer[0] = htonl(data->header.frame);
    *buffer[4] = data->header.type;
    *buffer[5] = htons(data->header.size);
    *buffer[7] = (char)0;
    /* Body */

    for (j; j < data->header.size; j++)
    {
        *buffer[8 * (j + 1)] = htons(data->body[j].entity_class);
        *buffer[8 * (j + 1) + 2] = htons(data->body[j].pos_x);
        *buffer[8 * (j + 1) + 2] = htons(data->body[j].pos_y);
        *buffer[8 * (j + 1) + 1] = data->body[j].sprite;
        *buffer[8 * (j + 1) + 1] = data->body[j].frame;
    }
}

int main()
{
    int sockfd;
    // char buffer[MAXLINE];
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
    struct data_t *data;
    int *size = 8;

    //creando socket descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Error al crear socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Llenado de la información del servidor
    servaddr.sin_family = AF_INET; //ipv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Vincular el socket con la dirección del servidor
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Falla al vincular");
        exit(EXIT_FAILURE);
    }
    data_to_buffer(&data, &buffer, &size);
    printf("Despues de buffer: %s\n", &buffer[0]);
    int len, n;

    len = sizeof(cliaddr);
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);

    buffer[n] = '\0';
    printf("Cliente: %s\n", buffer);
    sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    printf("Mensaje Hello enviado. \n");

    return 0;
}
