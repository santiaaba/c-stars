#include "eaeapp.h"

/******************************************************
 *							Para TCP									*
 ******************************************************/

void req_init(req_t *req, uint8_t cod, uint16_t size){
	req->header.cod = cod;
	req->header.qid = 0;		// POR EL MOMENTO
	req->header.aux = 0;
	req->header.size = size;
}

void res_init(res_t *res, uint8_t cod, uint8_t resp, uint16_t size){
	res->header.cod = cod;
	res->header.resp = resp;
	res->header.size = size;
	res->header.qid = 0;	// POR EL momento
}




void eaeapp_req_char2header(req_t *req, char *buffer, int size){
	req->header.cod = (uint8_t)buffer[0];
   req->header.aux = (uint8_t)buffer[1];
   req->header.size = ntohs(buffer[2]);
   req->header.qid = ntohl(buffer[4]);
}

void eaeapp_req_char2body(req_t *req, char *buffer, int size){
	if(req->body != NULL)
		free(req->body);
	switch(req->header.cod){
		case C_CONNECT_1:
			req->body = (req_connect_t*)malloc(sizeof(req_connect_t));
			((req_connect_t*)(req->body))->udp = ntohs(buffer[8]);
			((req_connect_t*)(req->body))->version = ntohs(buffer[10]);
			break;
		case C_KEY_PRESS:
			req->body = (req_kp_t*)malloc(sizeof(req_kp_t));
			((req_kp_t*)(req->body))->key = ntohs(buffer[8]);
			((req_kp_t*)(req->body))->action = ntohs(buffer[10]);
			break;
	}
}

void eaeapp_req_header2char(req_t *req, char *buffer, int *size){}
void eaeapp_req_body2char(req_t *req, char *buffer, int *size){}



void eaeapp_res_header2char(res_t *res, char *buffer, int *size){
	*size = RES_HEAD_SIZE + res->header.size;
	*buffer = (char *)realloc(*buffer,(int long unsigned)(*size));
	*buffer[0] = res->header.cod;
	*buffer[1] = res->header.resp;
	*buffer[2] = htons(res->header.size);
	*buffer[4] = htonl(res->header.qid);
}

void eaeapp_res_body2char(res_t *res, char *buffer, int *size){
	switch(res->header.cod){
		case C_CONNECT_1:
			memcpy(&(*buffer)[8],res->body,res->header.size);
			break;
		case C_GAME_STATUS:
			f(buffer[8],res->body);
	}
}

void eaeapp_res_char2header(res_t *res, char *buffer, int size){}
void eaeapp_res_char2body(res_t *res, char *buffer, int size){}




void req_to_buffer(req_t *req, char **buffer, int *size){
	uint16_t aux;

	printf("size body: %i\n",req->header.size);
	*size = REQ_HEAD_SIZE + req->header.size;
	printf("entro %lu\n", (int long unsigned)(*size));

	(*buffer) = (char *)realloc(*buffer,(int long unsigned)(*size));
	(*buffer)[0] = (char unsigned)(req->header.cod);
	(*buffer)[1] = (char unsigned)(req->header.aux);
	(*buffer)[2] = htons(req->header.size);

	(*buffer)[4] = htonl(req->header.qid);

	switch(req->header.cod){
		case C_CONNECT_1:
			aux = htons(((req_connect_t*)(req->body))->udp);
			memcpy(&(*buffer)[8],&aux,2);
			printf("entro u4\n");
			aux = htons(((req_connect_t*)(req->body))->version);
			memcpy(&(*buffer)[10],&aux,2);
			printf("entro u5\n");
			break;
		case C_KEY_PRESS:
			aux = htons(((req_kp_t*)(req->body))->key);
			memcpy(&(*buffer)[8],&aux,2);
			aux = htons(((req_kp_t*)(req->body))->action);
			memcpy(&(*buffer)[10],&aux,2);
			break;
	}
	//(*buffer)[0] = 'A';
	//(*buffer)[1] = 'B';
	//(*buffer)[2] = 'A';
	//(*buffer)[3] = '\n';
}

int buffer_to_req(req_t *req, char *buffer, int size){

	printf("entro\n");

	req->header.cod = (uint8_t)buffer[0];
	req->header.aux = (uint8_t)buffer[1];
	req->header.size = ntohs(buffer[2]);
	req->header.qid = ntohl(buffer[4]);
	
	printf("Buffer_to_req\n");

	if(req->body != NULL)
		free(req->body);
	switch(req->header.cod){
		case C_CONNECT_1:
			req->body = (req_connect_t*)malloc(sizeof(req_connect_t));
			((req_connect_t*)(req->body))->udp = ntohs(buffer[8]);
			((req_connect_t*)(req->body))->version = ntohs(buffer[10]);
			break;
		case C_KEY_PRESS:
			req->body = (req_kp_t*)malloc(sizeof(req_kp_t));
			((req_kp_t*)(req->body))->key = ntohs(buffer[8]);
			((req_kp_t*)(req->body))->action = ntohs(buffer[10]);
			break;
	}
	return 1;
}

void res_to_buffer(res_t *res, char **buffer, int *size, void f(char*,void*)){

	*size = RES_HEAD_SIZE + res->header.size;
	*buffer = (char *)realloc(*buffer,(int long unsigned)(*size));
	*buffer[0] = res->header.cod;
	*buffer[1] = res->header.resp;
	*buffer[2] = htons(res->header.size);
	*buffer[4] = htonl(res->header.qid);

	switch(res->header.cod){
		case C_CONNECT_1:
			memcpy(&(*buffer)[8],res->body,res->header.size);
			break;
		case C_GAME_STATUS:
			f(buffer[8],res->body);
	}
}

int buffer_to_res(res_t *res, char *buffer, int size,void f(char*,void*)){

	res->header.cod = (uint8_t)buffer[0];
	res->header.resp = (uint8_t)buffer[1];
	res->header.size = ntohs(buffer[2]);
	res->header.qid = ntohl(buffer[4]);

	if(res->body != NULL)
		free(res->body);
	switch(res->header.cod){
		case C_CONNECT_1:
			res->body = (char*)malloc(res->header.size);
			memcpy(&(res->body),&buffer[8],res->header.size/8);
			break;
		case C_GAME_STATUS:
			f(&buffer[8],&(res->body));
	}
	return 1;
}

/******************************************************
 *							Para UDP									*
 ******************************************************/
void data_to_buffer(data_t *data, char **buffer, int *size){
	/* Convierte la estructura data en buffer. Evitamos
		tener que andar redimencionando continuamente el
	   buffer. Asi que size debe ser un reflejo fiel del
		tamaño del buffer */
	int j=0;

	/* Encabezado */
	if(*size < DATA_HEAD_SIZE + (data->header.size * 8)){
		*size = DATA_HEAD_SIZE + (data->header.size * 8);
		*buffer = (char *)realloc(*buffer,*size);
	}
	*buffer[0] = htonl(data->header.frame);
	*buffer[4] = data->header.type;
	*buffer[5] = htons(data->header.size);
	*buffer[7] = (char)0;
	/* Body */

	for(j;j<data->header.size;j++){
		*buffer[8 * (j+1)] = htons(data->body[j].entity_class);
		*buffer[8 * (j+1) + 2] = htons(data->body[j].pos_x);
		*buffer[8 * (j+1) + 2] = htons(data->body[j].pos_y);
		*buffer[8 * (j+1) + 1] = data->body[j].sprite;
		*buffer[8 * (j+1) + 1] = data->body[j].frame;
	}
}

void buffer_to_data(data_t *data, char *buffer, int size){
	/* Convierte la estructura buffer en data */
	int j=0;

	/* Encabezado */
	data->header.frame = ntohl(buffer[0]);
	data->header.type = buffer[4];
	data->header.size = ntohs(buffer[5]);
	data->header.aux = 0;

	/* body */
	for(j;j<data->header.size;j++){
		data->body[j].entity_class = ntohs(buffer[8 * (j+1)]);
		data->body[j].pos_x = (int)ntohs(buffer[8 * (j+1) + 2]);
		data->body[j].pos_y = (int)ntohs(buffer[8 * (j+1) + 2]);
		data->body[j].sprite = (uint8_t)buffer[8 * (j+1) + 1];
		data->body[j].frame = (uint8_t)buffer[8 * (j+1) + 1];
	}
}

void data_entity_copy(data_render_t *dest, data_render_t orig){
	dest->entity_class = orig.entity_class;
   dest->pos_x = orig.pos_x;
   dest->pos_y = orig.pos_y;
   dest->sprite = orig.sprite;
   dest->frame =  orig.frame;
}
