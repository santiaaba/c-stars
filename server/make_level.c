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
	int16_t i,j,k = 0;
	FILE *fdIN;
	FILE *fdOUT;

	fdIN = fopen(argv[1],"r");
	fdOUT = fopen(argv[2],"wb");

	/* Los tres primeros char determinan el nivel */
	fscanf(fdIN,"%hd",&cantAtaques);		/* Cantidad de ataques */
	fwrite(&cantAtaques,sizeof(int16_t),1,fdOUT);
	fscanf(fdIN,"%hd",&num);				/* ID Background */
	fwrite(&num,sizeof(int16_t),1,fdOUT);
	fscanf(fdIN,"%hd",&num);				/* ID Sound Track */
	fwrite(&num,sizeof(int16_t),1,fdOUT);

	/* Leemos los ataques */
	while(i < cantAtaques){
		fscanf(fdIN,"%hd",&cantShips);			/* Cantidad de naves */
		fwrite(&cantShips,sizeof(int16_t),1,fdOUT);
		fscanf(fdIN,"%hd",&num);			/* tiempo de inicio */
		fwrite(&num,sizeof(int16_t),1,fdOUT);

		/* Leemos las naves */
		j = 0;
		while(j < cantShips ){
			fscanf(fdIN,"%hd",&num);			/* Tipo de nave */
			fwrite(&num,sizeof(int16_t),1,fdOUT);
			fscanf(fdIN,"%hd",&num);			/* Coordenada x de inicio */
			fwrite(&num,sizeof(int16_t),1,fdOUT);
			fscanf(fdIN,"%hd",&num);			/* Coordenada y de inicio */
			fwrite(&num,sizeof(int16_t),1,fdOUT);
			fscanf(fdIN,"%hd",&num);			/* tiempo de inicio */
			fwrite(&num,sizeof(int16_t),1,fdOUT);
			fscanf(fdIN,"%hd",&cantMovements);			/* Cantidad movimientos */
			fwrite(&cantMovements,sizeof(int16_t),1,fdOUT);
			
			k = 0;
			while(k < cantMovements ){
				fscanf(fdIN,"%hd",&num);			/* modulo */
				fwrite(&num,sizeof(int16_t),1,fdOUT);
				fscanf(fdIN,"%hd",&num);			/* direccion */
				fwrite(&num,sizeof(int16_t),1,fdOUT);
				fscanf(fdIN,"%hd",&num);			/* diracion */
				fwrite(&num,sizeof(int16_t),1,fdOUT);
				k++;
			}
			j++;
		}
		i++;
	}

	fclose(fdIN);
	fclose(fdOUT);
}
