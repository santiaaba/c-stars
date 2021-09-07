#include "eaeapp.h"

/* Encargado de recibir y responder a las peticiones
   enviadas por el cliente */

int req_parse(req_t *req, char *buffer, int size){
	/* Parsea los datos del buffer a la estructura req_t.
		Retorna 1 si tuvo exito. Retorna 0 si no lo tuvo. */

	// Verificar que el buffer tenga al menos el tamano del encabezado
	//buffer[0]	// El primer char conforma el codigo y el aux.
					// ambos de 8 bits aca uno.
	//buffer[1]	// El siguiente char es el size del data
	//buffer[2]	// Es el qid entregado por el cliente
	
	// Verificar que el primer char este en rango
	// Verificar que el siguiente char este en rango
	// Verificar que el tercer char este en rango

	// Armar el encabezado

	//Dependiendo del codigo... armar el body a partir del 4to byte
}

int res_parse(res_t *res, char **buffer, int *size){
	/* Convierta una estructura de respuesta para ser enviada */
}
