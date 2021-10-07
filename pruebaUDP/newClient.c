// implementacion de servidor UDP
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
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
void buffer_to_header(data_header_t *header, char *buffer);
void buffer_to_render(data_render_t *body, char *buffer);
void gen_data(data_t *data);
int main()
{
    int size = BUFFER_SIZE;
    int sd;

    data_header_t header;
    data_render_t body;
    data_t data;
    char buffer[500];
    data.header.frame = 1;
    data.body->frame = 1;
    int bytes;

    socklen_t lon;
    struct sockaddr_in servidor_udp;
    struct sockaddr_in cliente_udp;

    sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    cliente_udp.sin_family = AF_INET;
    cliente_udp.sin_port = htons(12033);
    cliente_udp.sin_addr.s_addr = INADDR_ANY;

    lon = sizeof(struct sockaddr_in);

    if (bind(sd, (struct sockaddr *)&cliente_udp, lon) < 0)
    {
        perror("Error en bind");
        exit(-1);
    }

    // Recibe header
    bytes = recvfrom(sd, buffer, HEADER_SIZE, 0, (struct sockaddr *)&servidor_udp, &lon);
    if (bytes < 0 || bytes < 8)
    {
        printf("Error en la recepcion");
    }
    buffer_to_header(&(data.header), buffer);
    int total = 0;
    int i = 0;
    while (total < header.size)
    {
        bytes = recvfrom(sd, buffer, HEADER_SIZE, 0, (struct sockaddr *)&servidor_udp, &lon);
        buffer_to_render(&(data.body[i]), buffer);
        total += bytes;
        i++;
    }
    printf("el valor del frame del body es: %d\n", body.frame);
    close(sd);
    return 0;
}

void buffer_to_header(data_header_t *header, char *buffer)
{
    header->frame = buffer[0];
    header->type = buffer[4];
    header->size = buffer[5];
    header->aux = buffer[7];
}
void buffer_to_render(data_render_t *body, char *buffer)
{
    body->entity_class = buffer[0];
    body->pos_x = buffer[2];
    body->pos_y = buffer[4];
    body->sprite = buffer[5];
    body->frame = buffer[6];
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