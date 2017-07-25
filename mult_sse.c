/*
chamada do programa sera ./<nome do programa> <arquivo com matriz a> <arquivo com matriz b> <tamanho das matrizes>
*/

#include <stdio.h>
#include <time.h>
#include "matriz.h"

#define SAIDA "mult_sse.txt"

void scan_matriz (Matriz *matriz, FILE *arquivo) {
	float leitura;
	size_t i;

	for (i = 0; i < matriz->dim*matriz->dim; i++) {
		fscanf(arquivo, "%f", &matriz->data[i]);
	}
}

int main (int argc, char **args) {

	Matriz a, b, c;
	size_t dim;
	FILE *matriz_a, *matriz_b, *saida;

	clock_t inicio, fim;

	if (argc < 4) {
		printf ("Numero invalido de argumentos!\nChamada do programa: ./<nome do programa> <arquivo com matriz a> <arquivo com matriz b> <tamanho das matrizes>\n");
		return 1;
	}

	//leitura dos argumentos do programa
	matriz_a = fopen(args[1], "r");
	if (matriz_a == NULL) {
		printf("\"%s\" nao encontrado\n", args[1]);
		return 2;
	}
	matriz_b = fopen(args[2], "r");
	if (matriz_b == NULL) {
		printf("\"%s\" nao encontrado\n", args[2]);
		return 2;
	}
	sscanf(args[3], "%lu", &dim);

	//preparacao das matrizes com base nos argumentos do programa
	alocar(&a, dim);
	alocar(&b, dim);
	//alocar(&c, dim);
	scan_matriz(&a, matriz_a);
	scan_matriz(&b, matriz_b);

	fclose(matriz_a);
	fclose(matriz_b);

	//multiplicacao das matrizes
	inicio = clock();
	multTransp_sse(&c, &a, &b);
	fim = clock();

	//escrever resultado na saida
	saida = fopen (SAIDA, "w");
	//fprintf(saida, "MULTIPLICACAO %ux%u: %lf\n\n", dim, dim, (double)(fim - inicio) / CLOCKS_PER_SEC );
	matriz_fprintf(saida, &c);
	fclose(saida);


	//liberar espaco
	desalocar(&a);
	desalocar(&b);
	desalocar(&c);

	return 0;

} 
