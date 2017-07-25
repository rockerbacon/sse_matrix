#include <stdio.h>
#include "matriz.h"


void scan_matriz (Matriz *matriz, FILE *arquivo) {
	float leitura;
	size_t i;

	for (i = 0; i < matriz->dim*matriz->dim; i++) {
		fscanf(arquivo, "%f", &matriz->data[i]);
	}
}

void print_matriz (Matriz *matriz, FILE	*arquivo) {
	size_t i, j;
	for (i = 0; i < matriz->dim; i++) {
		for (j = 0; j < matriz->dim; j++) {
			fprintf(arquivo, "%.0f ", matriz->data[i*matriz->dim+j]);
		}
		fprintf(arquivo, "\n");
	}

}

int main(int argc, char **args) {

	Matriz a;
	size_t dim;

	int result;

	FILE *matriz_a;
	
	//leitura dos argumentos do programa
	matriz_a = fopen(args[1], "r");
	if (matriz_a == NULL) {
		printf("\"%s\" nao encontrado\n", args[1]);
		return 2;
	}

	sscanf(args[2], "%lu", &dim);

	//preparacao das matrizes com base nos argumentos do programa
	alocar(&a, dim);
	scan_matriz(&a, matriz_a);

	result = eh_ident_sse(&a);

	printf("Resultado = %d \n",result );

	return 0;
}