/*
chamada do programa sera ./<nome do programa> <arquivo de saida> <tamanho da matriz> <limite inferior> <limite superior>
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char **args) {

	if (argc < 5) {
		printf ("Numero invalido de argumentos!\nChamada do programa: ./<nome do programa> <arquivo de saida> <tamanho da matriz> <limite inferior> <limite superior>\n");
		return 1;
	}

	FILE *saida = fopen(args[1], "w");
	size_t dim, i, j;
	float limiteInferior, limiteSuperior, range, aleatorio;

	sscanf(args[2], "%lu", &dim);
	sscanf(args[3], "%f", &limiteInferior);
	sscanf(args[4], "%f", &limiteSuperior);

	srand(clock());
	
	range = limiteSuperior - limiteInferior;
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			aleatorio = (float)rand() / RAND_MAX  * range + limiteInferior;
			fprintf(saida, "%.2f ", aleatorio);
		}
		fprintf(saida, "\n");
	}

	return 0;

}
