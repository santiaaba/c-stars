/* Convierteuna serie de instrucciones
	como ASCII en binario */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h> 

void main(char argc, char * argv[]){

	int16_t num;
	int16_t cantAtaques;
	int16_t cantMovements;
	int16_t i,k = 0;
	FILE *fdIN;
	FILE *fdOUT;

	fdIN = fopen(argv[1],"r");
	fdOUT = fopen(argv[2],"wb");

	/* Los tres primeros char determinan el nivel */
	fscanf(fdIN,"%hd",&cantAtaques);		/* Cantidad de ataques */
	fwrite(&cantAtaques,sizeof(int16_t),1,fdOUT);
	printf("Cantidad de ataques: %i\n",cantAtaques);

	fscanf(fdIN,"%hd",&num);				/* ID Background */
	fwrite(&num,sizeof(int16_t),1,fdOUT);
	printf("BackGround: %i\n",num);

	fscanf(fdIN,"%hd",&num);				/* ID Sound Track */
	fwrite(&num,sizeof(int16_t),1,fdOUT);
	printf("SoundTrack: %i\n",num);

	i=0;
	while(i < cantAtaques){
		fscanf(fdIN,"%hd",&num);			/* Tipo de nave */
		printf("	tipo nave: %i - ",num);
		fwrite(&num,sizeof(int16_t),1,fdOUT);
		fscanf(fdIN,"%hd",&num);			/* Coordenada x de inicio */
		fwrite(&num,sizeof(int16_t),1,fdOUT);
		printf("	inicio: (%i,",num);
		fscanf(fdIN,"%hd",&num);			/* Coordenada y de inicio */
		fwrite(&num,sizeof(int16_t),1,fdOUT);
		printf("%i) - ",num);
		fscanf(fdIN,"%hd",&num);			/* tiempo de inicio */
		fwrite(&num,sizeof(int16_t),1,fdOUT);
		printf("tiempo: %i\n",num);
		fscanf(fdIN,"%hd",&cantMovements);			/* Cantidad movimientos */
		fwrite(&cantMovements,sizeof(int16_t),1,fdOUT);
		printf("Movimientos (%i)\n",cantMovements);
		k = 0;
		while(k < cantMovements ){
			fscanf(fdIN,"%hd",&num);			/* modulo */
			fwrite(&num,sizeof(int16_t),1,fdOUT);
			printf("	modulo: %i, ",num);
			fscanf(fdIN,"%hd",&num);			/* direccion */
			fwrite(&num,sizeof(int16_t),1,fdOUT);
			printf("direccion: %i, ",num);
			fscanf(fdIN,"%hd",&num);			/* tiempo */
			fwrite(&num,sizeof(int16_t),1,fdOUT);
			printf("momento: %i\n",num);
			k++;
		}
		i++;
	}

	fclose(fdIN);
	fclose(fdOUT);
}
