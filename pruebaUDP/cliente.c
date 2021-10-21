#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 200
#define MAX_DATA_BODY 50 // En elementos

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
void data_to_buffer(data_t *data, char *buffer);
void gen_data(data_t *data);

int main()
{
    int sockfd;
    char buffer[MAXLINE];
    data_header_t header;
    data_render_t body;
    data_t data;
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("error al crear el socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Llenado de la información del servidor
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // genero la data
    gen_data(&data);

    printf("%i", data.body->frame);
    printf("%i", data.body->pos_x);
    printf("%i", data.body->pos_y);
    printf("%i", data.body->entity_class);

    printf("\n header: %i", data.header.aux);
    printf("%i", data.header.frame);
    printf("%i", data.header.size);
    printf("%i", data.header.type);

    data_to_buffer(&(data), buffer);

    int n, len;

    sendto(sockfd, (char *)buffer, 16, MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    close(sockfd);
    return 0;
}

void gen_data(data_t *data)
{

    data->header.frame = 1;
    data->header.type = 1;
    data->header.size = 8; // cantidad de bytes del body
    data->header.aux = 0;

    data->body[0].entity_class = 1;
    data->body[0].pos_x = 3;
    data->body[0].pos_y = 4;
    data->body[0].sprite = 5;
    data->body[0].frame = 2;
}

void data_to_buffer(data_t *data, char *buffer)
{
    uint32_t aux32;
    uint16_t aux16;

    // Header
    aux32 = htonl(data->header.frame);
    memcpy(&(buffer[0]), &aux32, 4); // convierto un numero de 32 bits como 4 caracteres dentro del buffer
    buffer[4] = data->header.type;   // es la forma de guardar en buffer un byte
    aux16 = htons(data->header.size);
    memcpy(&(buffer[5]), &aux16, 2); // convierto un numero de 16 bits como 2    caracteres dentro del buffer
    buffer[7] = data->header.aux;

    // Body
    int i = 8;
    aux16 = htons(data->body[0].entity_class);
    memcpy(&(buffer[i]), &aux16, 2);
    aux16 = htons(data->body[0].pos_x);
    memcpy(&(buffer[i + 2]), &aux16, 2);
    aux16 = htons(data->body[0].pos_y);
    memcpy(&(buffer[i + 4]), &aux16, 2);
    buffer[i + 6] = data->body[0].sprite;
    buffer[i + 7] = data->body[0].frame;
}
