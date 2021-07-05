/* Convierteuna serie de instrucciones
	como ASCII en binario */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h> 

void main(char argc, char * argv[]){

	int16_t num;
	int16_t cantAtaques;
	int16_t cantShips;
	int16_t cantMovements;
	int16_t i,j,k;
	FILE *fdIN;

	fdIN = fopen(argv[1],"rb");

	/* Los tres primeros char determinan el nivel */
	fread(&cantAtaques,sizeof(int16_t),1,fdIN);	/* Cantidad de ataques */
	printf("%d\n",cantAtaques);
	fread(&num,sizeof(int16_t),1,fdIN);				/* ID Background */
	printf("%d\n",num);
	fread(&num,sizeof(int16_t),1,fdIN);				/* ID Sound Track */
	printf("%d\n",num);

	/* Leemos los ataques */
	i = 0;
	while(i < cantAtaques){
		printf("------   ATAQUE   ------\n");
		fread(&cantShips,sizeof(int16_t),1,fdIN);	/* Cantidad de naves */
		printf("%d\n",cantShips);
		fread(&num,sizeof(int16_t),1,fdIN);			/* tiempo de inicio */
		printf("%d\n",num);

		/* Leemos las naves */
		j = 0;
		while(j < cantShips ){
			printf("------   NAVE   ------\n");
			fread(&num,sizeof(int16_t),1,fdIN);			/* Tipo de nave */
			printf("%d\n",num);
			fread(&num,sizeof(int16_t),1,fdIN);			/* Coordenada x */
			printf("%d\n",num);
			fread(&num,sizeof(int16_t),1,fdIN);			/* Coordenada y */
			printf("%d\n",num);
			fread(&num,sizeof(int16_t),1,fdIN);			/* Tiempo inicio */
			printf("%d\n",num);
			fread(&cantMovements,sizeof(int16_t),1,fdIN);			/* cantidad movimientos */
			printf("%d\n",cantMovements);

			k = 0;
			while(k < cantMovements ){
				printf("------   NAVE Movimiento   ------\n");
				fread(&num,sizeof(int16_t),1,fdIN);			/* modulo */
				printf("%d\n",num);
				fread(&num,sizeof(int16_t),1,fdIN);			/* direccion */
				printf("%d\n",num);
				fread(&num,sizeof(int16_t),1,fdIN);			/* duracion */
				printf("%d\n",num);
				k++;
			}
			j++;
		}
		i++;
	}

	fclose(fdIN);
}
