CC=gcc
CFLAGS=-msse4.1
LFLAGS=-lm

%.o: %.c
	$(CC) -c $^ -o $@ $(CFLAGS)

all: bench_mult mult bench_mult_sse mult_sse iden_sse

bench_mult: matriz.o bench_mult.o
	$(CC) -o $@ $^ $(LFLAGS)

mult: matriz.o mult.o
	$(CC) -o $@ $^ $(LFLAGS)

bench_mult_sse: matriz.o bench_mult_sse.o
	$(CC) -o $@ $^ $(LFLAGS)

mult_sse: matriz.o mult_sse.o
	$(CC) -o $@ $^ $(LFLAGS)

matriz_generator: matriz_generator.c
	$(CC) -o $@ $<

iden_sse: matriz.o iden_sse.o
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	rm -f *.o
	rm -f bench_mult
	rm -f mult
	rm -f bench_mult_sse
	rm -f mult_sse
	rm -f matriz_generator
	rm -f iden_sse
