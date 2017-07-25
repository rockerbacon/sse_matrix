/*
Operacoes utilizando matrizes quadradas
Nas funcoes que recebem parametros para guardar resultados os parametros passados devem ser alocados previamente a menos que dito o contrario
*/

#ifndef _MATRIZ_H_
#define _MATRIZ_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct {

	//atributos
	float *data;
	size_t dim;
	
} Matriz;

/*METODOS*/
//escrever matriz em file com 2 digitos de precisao
void matriz_fprintf (FILE *file, Matriz *matriz);

//inicializar matriz
void alocar (Matriz *matriz, size_t dim);

//deletar matriz
void desalocar (Matriz *matriz);

//copia matriz origem em destino
void copiar (Matriz *destino, Matriz *origem);

//copia matriz origem em destino (copia otimizada utilizando instrucoes sse
//matriz destino nao deve estar alocada antes da chamada dessa funcao
void copiar_sse (Matriz *destino, Matriz *origem);

//faz zero padding para que a matriz padded tenha dimensoes multiplas de multOf e possua os valores de matriz
//a matriz padded nao deve estar alocada antes da chamada dessa funcao
void pad (Matriz *padded, Matriz *matriz, size_t multOf);

//transpor matriz guardando os valores na tranposta
void transp (Matriz *tranposta, Matriz *matriz);

//transpor matriz guardando os valores na transposta (transposicao otimizada utilizando instrucoes sse)
//Matriz transposta sera padded e nao deve estar alocada antes da chamada da funcao
void transp_sse (Matriz *transposta, Matriz *matriz);

//multiplicar a*b guardando os valores em c
void mult (Matriz *c, Matriz *a, Matriz *b);

//multiplicar a*b guardando os valores em c (transpoe b para otimizar cache)
void multTrasp (Matriz *c, Matriz *a, Matriz *b);

//multiplicar a*b guardando os valores em c (multiplicacao otimizada utilizando instrucoes sse)
//matriz c nao deve ser alocada antes da chamada da funcao
void multTransp_sse (Matriz *c, Matriz *a, Matriz *b);

//transforma matriz em matriz identidade
void set_ident (Matriz *matriz);

//verificar se matriz eh identidade, retorna 1 se sim e 0 se nao
int eh_ident (Matriz *matriz);

//verifica se a matriz eh identidade utilizando sse
int eh_ident_sse (Matriz *matriz);

#endif
