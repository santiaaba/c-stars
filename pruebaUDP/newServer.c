// implementacion de servidor UDP
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8889
#define BUFFER_SIZE 1024
#define MAX_DATA_BODY 200 // En elementos
#define DATA_HEAD_SIZE 8

#define HEADER_SIZE 8
#define RENDER_SIZE 8

typedef struct data_header_t data_header_t;
struct data_header_t
{
    uint32_t frame; // hasta 4294967295
    uint8_t type;   // hasta 255
    uint16_t size;  /* Cantidad de elementos del body en bytes. hasta 65535 */
    uint8_t aux;    // hasta 255
};

typedef struct data_render_t data_render_t;
struct data_render_t
{
    uint16_t entity_class; // hasta 65535
    int16_t pos_x;         // hasta 65535
    int16_t pos_y;         // hasta 65535
    uint8_t sprite;        // hasta 255
    uint8_t frame;         // hasta 255
};

typedef struct data_t data_t;
struct data_t
{
    data_header_t header;
    data_render_t body[MAX_DATA_BODY];
};
void header_to_buffer(data_header_t *header, char *buffer);
void render_to_buffer(data_render_t *body, char *buffer);
void gen_data(data_t *data);
int main(int argc, char *argv[])
{
    int size = BUFFER_SIZE;
    int sd;
    int sockfd;

    data_header_t header;
    data_render_t body;
    data_t data;
    char buffer[500];
    data.header.frame = 1;
    data.body->frame = 1;

    socklen_t lon;
    struct sockaddr_in cliente_udp;
    struct hostent *h;

    if (argc < 2)
    {
        printf("Debe ejecutar %s (nombre de host)\n", argv[0]);
        exit(-1);
    }

    sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    cliente_udp.sin_family = AF_INET;
    cliente_udp.sin_port = htons(12033);
    if (h = gethostbyname(argv[1]))
    {
        memcpy(&cliente_udp.sin_addr, h->h_addr, h->h_length);
    }
    else
    {
        herror("DNS error");
        exit(-1);
    }
    // Envío de buffer
    gen_data(&data);
    printf("tamaño data %ld\n", sizeof(data));
    // printf("puntero: %d\n", data.header.frame);

    // Primero enviamos el encabezado
    header_to_buffer(&(data.header), buffer);

    // sendto(sd, buffer, HEADER_SIZE, 0, (struct sockaddr *)&cliente_udp, lon);
    sendto(sd, buffer, HEADER_SIZE, 0, (struct sockaddr *)&cliente_udp, lon);

    for (int i = 0; i < 1; i++)
    {
        render_to_buffer(&(data.body[i]), buffer);
        sendto(sd, buffer, RENDER_SIZE, 0, (struct sockaddr *)&cliente_udp, lon);
        printf("Body enviado %i\n", i);
    }

    close(sd);
    return 0;
}

void gen_data(data_t *data)
{
    data->header.aux = 0;
    data->header.frame = 1;
    data->header.size = 8; // cantidad de bytes del body
    data->header.type = 1;

    data->body[0].entity_class = 1;
    data->body[0].frame = 1;
    data->body[0].pos_x = 1;
    data->body[0].pos_y = 1;
    data->body[0].sprite = 1;
}
void header_to_buffer(data_header_t *header, char *buffer)
{
    buffer[0] = htonl(header->frame);
    buffer[4] = header->type;
    buffer[5] = htons(header->size);
    buffer[7] = (char)0;
}

void render_to_buffer(data_render_t *body, char *buffer)
{
    buffer[0] = htons(body->entity_class);
    buffer[2] = htons(body->pos_x);
    buffer[4] = htons(body->pos_y);
    buffer[5] = body->sprite;
    buffer[6] = body->frame;
}

/*

htons host to network short
ntohs network to host short
htonl host to network long
ntohl network to host long

The htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.

The ntohl() function converts the unsigned integer netlong from network byte order to host byte order.

The ntohs() function converts the unsigned short integer netshort from network byte order to host byte order.

long is 32 bits. short is 16 bits.

*/