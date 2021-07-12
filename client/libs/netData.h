/* Define estructuras y funciones para el manejo de datos enviados
   y recibidos del servidor */

#ifndef NET_DATA_H
#define NAT_DATA_H

typedef struct t_net_entity{
	uint8_t type;		/* Tipo de entidad */
	uint8_t sprite;	/* Dentro de la textura determina el sprite */
	uint8_t frame;		/* Determina el cuadro de la animación */
	int16_t x;			/* Coordenada x */
	int16_t y;			/* Coordenada y */
	
} net_entity_t

#endif
