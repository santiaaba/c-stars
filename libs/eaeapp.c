#include "eaeapp.h"

void printb(char *buffer, int size){
	for(int i=0;i<size;i++)
		printf("%i -> %i:%c\n",i,buffer[i],buffer[i]);
}

void print_header_req(req_t *req){
	printf("-----------------------------------------------------\n");
	printf("COD: %u | AUX: %u | SIZE: %u | QID: %"PRIu32"\n",
	req->header.cod,req->header.aux,req->header.size,req->header.qid);
	printf("-----------------------------------------------------\n");
}

void print_header_res(res_t *res){
	printf("-----------------------------------------------------\n");
	printf("COD: %u | RESP: %u | SIZE: %u | QID: %"PRIu32"\n",
	res->header.cod,res->header.resp,res->header.size,res->header.qid);
	printf("-----------------------------------------------------\n");
}

/******************************************************
 *							Para TCP									*
 ******************************************************/
void req_init(req_t *req){
	srand(time(NULL));
	req->header.cod = 0;
	req->header.qid = 0;
	req->header.aux = 0;
	req->header.size = 0;
	req->body = NULL;
}

void res_init(res_t *res){
	res->header.cod = 0;
	res->header.resp = 0;
	res->header.size = 0;
	res->header.qid = 0;
	res->body = NULL;
}

void req_destroy(req_t *req){
	if(req->body!=NULL){
		free(req->body);
		req->body = NULL;
	}
}

void req_fill(req_t *req, uint8_t cod, uint16_t size){
	req->header.qid = rand();
	req->header.cod = cod;
	req->header.size = size;
//	req_destroy(req);
}

void res_destroy(res_t *res){
	if(res->body!=NULL){
		free(res->body);
		res->body = NULL;
	}
}

void res_fill(res_t *res, uint8_t cod, uint8_t resp, uint16_t size, uint32_t qid){
	res->header.cod = cod;
	res->header.resp = resp;
	res->header.qid = qid;
	res->header.size = size;
	res_destroy(res);
}


void eaeapp_req2char(req_t *req, char *buffer, int *size){
	uint16_t aux16;
	uint32_t aux32;
	buffer[0] = req->header.cod;
	buffer[1] = req->header.aux;
	aux16 = htons(req->header.size);
	memcpy(&(buffer[2]),&aux16,2);
	aux32 = htonl(req->header.qid);
	memcpy(&(buffer[4]),&aux32,4);


	*size = REQ_HEADER_SIZE + req->header.size;
	print_header_req(req);
	switch(req->header.cod){
		case C_CONNECT_1:
			aux16 = htons(((req_connect_t*)(req->body))->udp);
			memcpy(&buffer[8],&aux16,2);
			aux16 = htons(((req_connect_t*)(req->body))->version);
			memcpy(&buffer[10],&aux16,2);
			break;
		case C_KEY_PRESS:
			aux16 = htons(((req_kp_t*)(req->body))->key);
			memcpy(&buffer[8],&aux16,2);
			aux16 = htons(((req_kp_t*)(req->body))->action);
			memcpy(&buffer[10],&aux16,2);
			printf("KEYPRESS - KEY:%i, EVENT:%i\n",
				((req_kp_t*)(req->body))->key,
				((req_kp_t*)(req->body))->action);
			break;
	}
}

void eaeapp_res2char(res_t *res, char *buffer, int *size){
	uint16_t aux16;
	uint32_t aux32;
	buffer[0] = res->header.cod;
	buffer[1] = res->header.resp;
	aux16 = htons(res->header.size);
	memcpy(&(buffer[2]),&aux16,2);
	aux32 = htonl(res->header.qid);
	memcpy(&(buffer[4]),&aux32,4);

	print_header_res(res);

	*size = RES_HEADER_SIZE + res->header.size;
	switch(res->header.cod){
		case C_CONNECT_1:
			memcpy(&buffer[8],res->body,res->header.size);
			break;
		case C_GAME_STATUS:
			printf("BODY: score= %"PRIu32"| state= %i| power=%i| level= %i| level_state= %i\n",
				((res_info_t*)(res->body))->score,
				((res_info_t*)(res->body))->state,
				((res_info_t*)(res->body))->power,
				((res_info_t*)(res->body))->level,
				((res_info_t*)(res->body))->level_state);
			aux32 = htonl(((res_info_t*)(res->body))->score);
			memcpy(&(buffer[8]),&aux32,4);

			buffer[12] = ((res_info_t*)(res->body))->state;
			buffer[13] = ((res_info_t*)(res->body))->power;
/*
			aux16 = htons(((res_info_t*)(res->body))->state);
			memcpy(&(buffer[12]),&aux16,2);
*/
			buffer[14] = ((res_info_t*)(res->body))->level;
			buffer[15] = ((res_info_t*)(res->body))->level_state;
	}
}

int eaeapp_char2req(req_t *req, char *buffer){
	uint16_t aux16;
	uint32_t aux32;
	req->header.cod = buffer[0];
   req->header.aux = buffer[1];
	memcpy(&aux16,&(buffer[2]),2);
	req->header.size = ntohs(aux16);
	memcpy(&aux32,&(buffer[4]),4);
	req->header.qid = ntohl(aux32);

	print_header_req(req);

	if(req->body != NULL){
		free(req->body);
		req->body = NULL;
	}
	switch(req->header.cod){
		case C_CONNECT_1:
			req->body = (req_connect_t*)malloc(sizeof(req_connect_t));
			memcpy(&aux16,&(buffer[8]),2);
			((req_connect_t*)(req->body))->udp = ntohs(aux16);
			memcpy(&aux16,&(buffer[10]),2);
			((req_connect_t*)(req->body))->version = ntohs(aux16);
			printf("UDP: %u | VERSION: %u\n",
				((req_connect_t*)(req->body))->udp,
				((req_connect_t*)(req->body))->version);
			return 12;
		case C_KEY_PRESS:
			req->body = (req_kp_t*)malloc(sizeof(req_kp_t));
			memcpy(&aux16,&(buffer[8]),2);
			((req_kp_t*)(req->body))->key = ntohs(aux16);
			memcpy(&aux16,&(buffer[10]),2);
			((req_kp_t*)(req->body))->action = ntohs(aux16);
			printf("KEY: %u | ACTION: %u\n",
				((req_kp_t*)(req->body))->key,
				((req_kp_t*)(req->body))->action);
			return 12;
		default:
			return 8;
	}
}


void eaeapp_char2res(res_t *res, char *buffer){
	uint16_t aux16;
	uint32_t aux32;
	res->header.cod = (uint8_t)buffer[0];
	res->header.resp = (uint8_t)buffer[1];
	memcpy(&aux16,&(buffer[2]),2);
	res->header.size = ntohs(aux16);
	memcpy(&aux32,&(buffer[4]),4);
	res->header.qid = ntohl(aux32);

	print_header_res(res);

	if(res->body != NULL){
		free(res->body);
		res->body = NULL;
	}
	switch(res->header.cod){
		case C_CONNECT_1:
			res->body = (char*)malloc(res->header.size);
			//memcpy(&(res->body),&buffer[8],res->header.size/8); /8 ????
			memcpy(&(res->body),&buffer[8],res->header.size);
			break;
		case C_GAME_STATUS:
			res->body = (res_info_t*)malloc(sizeof(res_info_t));
			memcpy(&aux32,&(buffer[8]),4);
			((res_info_t*)(res->body))->score = htonl(aux32);
/*
			memcpy(&aux16,&(buffer[12]),2);
			((res_info_t*)(res->body))->state = htons(aux16);
*/
			((res_info_t*)(res->body))->state = buffer[12];
			((res_info_t*)(res->body))->power = buffer[13];

			((res_info_t*)(res->body))->level = buffer[14];
			((res_info_t*)(res->body))->level_state = buffer[15];
			printf("SCORE:%"PRIu32" | STATE:%u | POWER:%u | LEVEL:%u | LEVEL_STATE: %u",
				((res_info_t*)(res->body))->score,
				((res_info_t*)(res->body))->state,
				((res_info_t*)(res->body))->power,
				((res_info_t*)(res->body))->level,
				((res_info_t*)(res->body))->level_state);
	}
}

/******************************************************
 *							Para UDP									*
 ******************************************************/
void data_to_buffer(data_t *data, char **buffer, int *size){
	/* size es el tamaño resultado del buffer. luego de
	   redimencionado */
	uint32_t aux32;
	uint16_t aux16;

	/* Header */
	if(data->header.type == D_VIDEO)
		*size = DATA_HEAD_SIZE + (data->header.size * 8);
	else
		*size = DATA_HEAD_SIZE + (data->header.size * 2);
	*buffer = (char *)realloc(*buffer,*size);

//	printf("data_to_buffer(): frame = %"PRIu32"\n",data->header.frame);
	aux32 = htonl(data->header.frame);
	memcpy(&(*buffer)[0], &aux32, 4);
	(*buffer)[4] = data->header.type;
//	printf("pasando type: %i\n",data->header.type);
	aux16 = htons(data->header.size);
	memcpy(&(*buffer)[5], &aux16, 2);
//	printf("pasando size: %i\n",data->header.size);
	(*buffer)[7] = data->header.aux;
//	printf("pasando aux: %i\n",data->header.aux);

	/* Body */
	if(data->header.type == D_VIDEO){
//		printf("enviando video\n");
		int k = 8;
		for(int j=0;j<data->header.size;j++){
			aux16 = htons(data->body[j].entity_class);
			memcpy(&(*buffer)[k], &aux16, 2);
			aux16 = htons(data->body[j].pos_x);
			memcpy(&(*buffer)[k+2], &aux16, 2);
			aux16 = htons(data->body[j].pos_y);
			memcpy(&(*buffer)[k+4], &aux16, 2);
			(*buffer)[k+6] = data->body[j].sprite;
			(*buffer)[k+7] = data->body[j].frame;
			k += 8;
		}
	} else {
		int k = 2;
		for(int j=0;j<data->header.size;j++){
			aux16 = htons(data->sound[j]);
			memcpy(&(*buffer)[k], &aux16, 2);
			k += 2;
		}
	}
//	printf("data_to_buffer: paso todo\n");
}

void buffer_to_data(data_t *data, char *buffer){
	/* Convierte la estructura buffer en data.*/
	uint16_t aux16;
	uint32_t aux32;

	/* Header */
//	printf("buffer_to_data(): Entro\n");
	memcpy(&aux32, &(buffer[0]), 4);
	data->header.frame = ntohl(aux32);
	data->header.type = buffer[4];
	memcpy(&aux16, &(buffer[5]), 2);
	data->header.size = ntohs(aux16);
	data->header.aux = buffer[7];
//	printf("buffer_to_data(): FRAME:%"PRIu32" | TYPE:%u | SIZE:%u | AUX: %u\n",
//		data->header.frame, data->header.type, data->header.size, data->header.aux);
	/* body */

	if(data->header.type == D_VIDEO){
		int k = 8;
		for(int j=0;j<data->header.size;j++){
			memcpy(&aux16, &(buffer[k]), 2);
			data->body[j].entity_class = ntohs(aux16);
			memcpy(&aux16, &(buffer[k + 2]), 2);
			data->body[j].pos_x = ntohs(aux16);
			memcpy(&aux16, &(buffer[k + 4]), 2);
			data->body[j].pos_y = ntohs(aux16);
			data->body[j].sprite = buffer[k + 6];
			data->body[j].frame = buffer[k + 7];
			k += 8;
//			printf("buffer_to_data():CLASS: %u | (X,Y):(%u,%u) | SPRITE: %u| FRAME: %u\n",
//					data->body[j].entity_class,
//					data->body[j].pos_x, data->body[j].pos_y,
//					data->body[j].sprite,data->body[j].frame);
		}
	} else {
		int k = 2;
		for(int j=0;j<data->header.size;j++){
			memcpy(&aux16, &(buffer[k]), 2);
			data->sound[j] = ntohs(aux16);
//			printf("buffer_to_data(): sound: %i\n",data->sound[j]);
			k += 2;
		}
	}
}

void data_entity_copy(data_render_t *dest, data_render_t *orig){
	dest->entity_class = orig->entity_class;
   dest->pos_x = orig->pos_x;
   dest->pos_y = orig->pos_y;
   dest->sprite = orig->sprite;
   dest->frame = orig->frame;
}
