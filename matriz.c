#include "matriz.h"
#include <xmmintrin.h>
#include <pmmintrin.h>
#include <math.h>

#define ALIGNOF__M128 16

void matriz_fprintf (FILE *file, Matriz *matriz) {
	size_t i, j;
	for(i = 0; i < matriz->dim; i++) {
		for (j = 0; j < matriz->dim; j++) {
			fprintf(file, "%.2f ", matriz->data[i*matriz->dim + j]);
		}
		fprintf(file, "\n");
	}
}

void alocar (Matriz *matriz, size_t dim) {
	posix_memalign ((void**)&matriz->data, ALIGNOF__M128, dim*dim*sizeof(float));
	if (matriz->data == NULL) {
		printf("allocation error\n");
	}
	matriz->dim = dim;
}

void desalocar (Matriz *matriz) {
	free(matriz->data);
}

void copiar (Matriz *destino, Matriz *origem) {
	size_t i, j;

	for (i = 0; i < origem->dim; i++) {
		for (j = 0; j < origem->dim; j++) {
			destino->data[i*destino->dim+j] = origem->data[i*origem->dim+j];
		}
	}
}

void pad (Matriz *padded, Matriz *matriz, size_t multOf) {
	size_t novaDim = matriz->dim - matriz->dim%multOf + multOf;
	size_t i, j;
	
	alocar (padded, novaDim);
	for (i = 0; i < matriz->dim; i++) {
		for (j = 0; j < matriz->dim; j++) {
			padded->data[i*padded->dim+j] = matriz->data[i*matriz->dim+j];
		}
	}
	
	for (i = 0; i < matriz->dim; i++) {
		for (j = matriz->dim; j < padded->dim; j++) {
			padded->data[i*padded->dim+j] = 0;
		}
	}
	for ( ; i < padded->dim; i++) {
		for (j = 0; j < padded->dim; j++) {
			padded->data[i*padded->dim+j] = 0;
		}
	}
}

void copiar_sse (Matriz *destino, Matriz *origem) {
	size_t i, j;
	__m128 temp;

	if (origem->dim%4 != 0) {
		pad(destino, origem, 4);
	} else {
		alocar(destino, origem->dim);
		for (i = 0; i < origem->dim; i++) {
			for (j = 0; j < origem->dim; j+=4) {
				temp = _mm_load_ps(origem->data + i*origem->dim+j);
				_mm_store_ps(destino->data + i*destino->dim+j, temp);
			}
		}
	}
}

void transp (Matriz *transposta, Matriz *matriz) {
	size_t i, j;
	for (i = 0; i < matriz->dim; i++) {
		for (j = 0; j < matriz->dim; j++) {
			transposta[i*matriz->dim+j] = matriz[j*matriz->dim+i];
		}
	}
}

void transp_sse (Matriz *transposta, Matriz *matriz) {

	size_t i, j;
	float *ij[4], *ji[4];
	__m128 lij[4], lji[4];

	copiar_sse(transposta, matriz);

	for (i = 0; i < transposta->dim; i += 4) {
		for (j = i; j < transposta->dim; j += 4) {

			//determinar submatriz 4x4 dentro de transposta na posicao ij
			ij[0] = transposta->data + i*transposta->dim+j;
			ij[1] = ij[0] + transposta->dim;
			ij[2] = ij[1] + transposta->dim;
			ij[3] = ij[2] + transposta->dim;

			lij[0] = _mm_load_ps(ij[0]);
			lij[1] = _mm_load_ps(ij[1]);
			lij[2] = _mm_load_ps(ij[2]);
			lij[3] = _mm_load_ps(ij[3]);

			//transpor submatriz ij
			_MM_TRANSPOSE4_PS(lij[0], lij[1], lij[2], lij[3]);

			if (i != j) {

				//determinar submatriz 4x4 dentro da transposta na posicao ji
				ji[0] = transposta->data + j*transposta->dim+i;
				ji[1] = ji[0] + transposta->dim;
				ji[2] = ji[1] + transposta->dim;
				ji[3] = ji[2] + transposta->dim;

				lji[0] = _mm_load_ps(ji[0]);
				lji[1] = _mm_load_ps(ji[1]);
				lji[2] = _mm_load_ps(ji[2]);
				lji[3] = _mm_load_ps(ji[3]);

				//traspor submatriz ji
				_MM_TRANSPOSE4_PS(lji[0], lji[1], lji[2], lji[3]);

				//trocar submatriz em ij com submatriz em ji
				//ij = ji
				_mm_store_ps(ij[0], lji[0]);
				_mm_store_ps(ij[1], lji[1]);
				_mm_store_ps(ij[2], lji[2]);
				_mm_store_ps(ij[3], lji[3]);

				//ji = ij
				_mm_store_ps(ji[0], lij[0]);
				_mm_store_ps(ji[1], lij[1]);
				_mm_store_ps(ji[2], lij[2]);
				_mm_store_ps(ji[3], lij[3]);
				
			} else {
				//guardar matriz transposta
				_mm_store_ps(ij[0], lij[0]);
				_mm_store_ps(ij[1], lij[1]);
				_mm_store_ps(ij[2], lij[2]);
				_mm_store_ps(ij[3], lij[3]);

			}

		}
	}
	
}

void mult (Matriz *c, Matriz *a, Matriz *b) {

	size_t i, j, k;

	for (i = 0; i < a->dim; i++) {
		for (j = 0; j < a->dim; j++) {
			c->data[i*c->dim+j] = 0;
			for (k = 0; k < a->dim; k++) {
				c->data[i*c->dim+j] += a->data[i*a->dim+k] * b->data[k*b->dim+j];
			}
		}
	}

}

void multTransp (Matriz *c, Matriz *a, Matriz *b) {
	
	Matriz bt;
	size_t i, j, k;
	
	alocar(&bt, b->dim);
	transp(&bt, b);
	
	for (i = 0; i < a->dim; i++) {
		for (j = 0; j < a->dim; j++) {
			c->data[i*c->dim+j] = 0;
			for (k = 0; k < a->dim; k++) {
				c->data[i*c->dim+j] += a->data[i*a->dim+k] * bt.data[j*bt.dim+k];
			}
		}
	}
	
	desalocar(&bt);
	
}

float somatorio_sse (float *data, size_t dim) {
	
	__m128 a, b;
	size_t i;
	
	a = _mm_load_ps(data);
	for (i = 4; i < dim; i += 4) {
		b = _mm_load_ps(data+i);
		a = _mm_add_ps(a, b);
	}
	a = _mm_hadd_ps(a, a);
	a = _mm_hadd_ps(a, a);
	
	return _mm_cvtss_f32(a);
}

void multTransp_sse (Matriz *c, Matriz *a, Matriz *b) {
	Matriz bt;
	Matriz ap;
	float *buffer;
	__m128 cursorA, cursorB, multBuffer;
	size_t i, j, k;
	
	transp_sse(&bt, b);
	if (a->dim != bt.dim) {
		pad(&ap, a, 4);
	} else {
		ap.data = a->data;
		ap.dim = a->dim;
	}
	alocar(c, ap.dim);
	
	posix_memalign((void**)&buffer, ALIGNOF__M128, sizeof(float)*ap.dim);
	for (i = a->dim; i < ap.dim; i++) {
		buffer[i] = 0;
	}

	for (i = 0; i < ap.dim; i++) {
		for (j = 0; j < ap.dim; j++) {
			for (k = 0; k < ap.dim; k += 4) {
				cursorA = _mm_load_ps(ap.data + i*ap.dim+k);
				cursorB = _mm_load_ps(bt.data + j*bt.dim+k);
				multBuffer = _mm_mul_ps(cursorA, cursorB);
				_mm_store_ps(buffer+k, multBuffer);
			}
			c->data[i*c->dim+j] = somatorio_sse(buffer, ap.dim);
		}
	}
}

void set_ident (Matriz *matriz) {
	size_t i, j;

	for (i = 0; i < matriz->dim; i++) {
		for (j = 0; j < matriz->dim; j++) {
			if (i != j) {
				matriz->data[i*matriz->dim+j] = 0;
			} else {
				matriz->data[i*matriz->dim+j] = 1;
			}
		}
	}

}

int eh_ident (Matriz *matriz) {
	size_t i, j;

	for (i = 0; i < matriz->dim; i++) {
		for (j = 0; j < matriz->dim; j++) {
			if (i != j) {
				if (matriz->data[i*matriz->dim+j] != 0) {
					return 0;
				}
			} else if (matriz->data[i*matriz->dim+j] != 1) {
				return 0;
			}
		}
	}

	return 1;
}

/*Retorna 0 se igual, diferente de 0 caso contrario*/
int comparar_sse (__m128 a, __m128 b) {	
	__m128 vcmp = _mm_cmpneq_ps(a, b); // compare a, b for inequality	
	return _mm_movemask_ps(vcmp); // extract results of comparison
}

int eh_ident_sse (Matriz *matriz){
	size_t i, j;
 	__m128 identidade[5],
           temp;
  
	float posicoes[20] = {1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1,
                     	  0, 0, 0, 0};
	
	identidade[0] = _mm_load_ps(posicoes);
	identidade[1] = _mm_load_ps(posicoes + 4);
	identidade[2] = _mm_load_ps(posicoes + 8);
	identidade[3] = _mm_load_ps(posicoes + 12);
	identidade[4] = _mm_load_ps(posicoes + 16);

	Matriz auxiliar;
    if(matriz->dim % 4 != 0) {
    	pad(&auxiliar, matriz, 4);
   		for(i = matriz->dim; i < auxiliar.dim; i++)
        	auxiliar.data[i*auxiliar.dim + i] = 1;
  	} else {
    	auxiliar.data = matriz->data;
		auxiliar.dim = matriz->dim;
	}
	for(i = 0; i < auxiliar.dim; i++) {
		for(j = 0; j < auxiliar.dim; j += 4) {
        	temp = _mm_load_ps(auxiliar.data + i*auxiliar.dim + j);
      
          	/*Fazer comparação*/ /*Se a comparação for diferente para qualquer caso retorna 0 */
          	if( !(j <= i && i < j + 4) ){
            		if(comparar_sse(temp, identidade[4]))
                    	return 0;
            } else if(comparar_sse(temp, identidade[i % 4])){
            	return 0;
            }
        }
    }
	
	return 1;
}
