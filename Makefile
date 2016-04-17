CC=gcc
CC_OPTIONS=-std=c99

EXE=bin/pbs bin/pfe
OBJ=bin/pbs.o bin/pfe.o bin/fatSupport.o

all: $(EXE)
	@make validate-build	

bin/fatSupport.o: src/utils/fatSupport.h src/utils/fatSupport.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c src/utils/fatSupport.c

bin/utilities.o: src/utils/utilities.h src/utils/utilities.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c src/utils/utilities.c

bin/pbs.o: src/prog/pbs.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c src/prog/pbs.c

bin/pfe.o: src/prog/pfe.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c src/prog/pfe.c

bin/pbs: bin/pbs.o bin/fatSupport.o bin/utilities.o
	@make validate-build
	$(CC) bin/pbs.o bin/fatSupport.o bin/utilities.o -o bin/pbs

bin/pfe: bin/pfe.o bin/fatSupport.o bin/utilities.o
	@make validate-build
	$(CC) bin/pfe.o bin/fatSupport.o bin/utilities.o -o bin/pfe

run:
	@make all

clean:
	rm -f $(EXE) $(OBJ) 

validate-build:
	if ! [ -d "./bin" ]; then mkdir -p "./bin"; fi
