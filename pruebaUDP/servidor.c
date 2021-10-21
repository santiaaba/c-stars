// implementacion de servidor UDP que para el juego sería el cliente que recibe los datagramas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <inttypes.h>

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

void buffer_to_data(data_t *data, char *buffer);

int main()
{
    int sockfd;
    char buffer[MAXLINE];
    data_t data;
    struct sockaddr_in servaddr, cliaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int len, n;

    len = sizeof(cliaddr);

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *)&cliaddr,
                 &len);
    printf("recibidos: %i\n", n);
    buffer_to_data(&(data), buffer);

    printf("header: %" PRIu32 "|", data.header.frame);
    printf("%u|", data.header.type);
    printf("%u|", data.header.size);
    printf("%u\n", data.header.aux);

    printf("body: %u|", data.body[0].entity_class);
    printf("%u|", data.body[0].pos_x);
    printf("%u|", data.body[0].pos_y);
    printf("%u|", data.body[0].sprite);
    printf("%u\n", data.body[0].frame);

    return 0;
}

void buffer_to_data(data_t *data, char *buffer)
{
    uint16_t aux16;
    uint32_t aux32;

    // Header
    memcpy(&aux32, &(buffer[0]), 4);
    data->header.frame = htonl(aux32);
    data->header.type = buffer[4];
    memcpy(&aux16, &(buffer[5]), 2);
    data->header.size = htons(aux16);
    data->header.aux = buffer[7];

    // Body
    int i = 8;
    memcpy(&aux16, &(buffer[i]), 2);
    data->body[0].entity_class = htons(aux16);
    memcpy(&aux16, &(buffer[i + 2]), 2);
    data->body[0].pos_x = htons(aux16);
    memcpy(&aux16, &(buffer[i + 4]), 2);
    data->body[0].pos_y = htons(aux16);
    data->body[0].sprite = buffer[i + 6];
    data->body[0].frame = buffer[i + 7];
}