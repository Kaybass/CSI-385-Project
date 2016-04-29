CC=gcc
CC_OPTIONS=-std=c99 -D _SVID_SOURCE

EXE=bin/pbs bin/pfe bin/ls bin/cat bin/MASH bin/cd
OBJ=bin/pbs.o bin/pfe.o bin/ls.o bin/cat.o bin/fatSupport.o bin/utilities.o bin/cd.o

all: $(EXE)
	@make validate-build

bin/fatSupport.o: src/utils/fatSupport.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c $^

bin/utilities.o: src/utils/utilities.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c $^

bin/pbs.o: src/prog/pbs.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c $^

bin/pfe.o: src/prog/pfe.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c $^

bin/ls.o: src/prog/ls.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c $^

bin/cat.o: src/prog/cat.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c $^

bin/cd.o: src/prog/cd.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c $^

bin/mash.o: src/mash/shell.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c $^

bin/main.o: src/main.c
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) -c $^

bin/pbs: bin/pbs.o bin/fatSupport.o bin/utilities.o
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) $^

bin/pfe: bin/pfe.o bin/fatSupport.o bin/utilities.o
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) $^

bin/ls: bin/ls.o bin/fatSupport.o bin/utilities.o
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) $^

bin/cat: bin/cat.o bin/fatSupport.o bin/utilities.o
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) $^

bin/cd: bin/cd.o bin/fatSupport.o bin/utilities.o
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) $^

bin/MASH: bin/mash.o bin/main.o
	@make validate-build
	$(CC) -o $@ $(CC_OPTIONS) $^

run:
	@make all

clean:
	rm -f $(EXE) $(OBJ)

validate-build:
	if ! [ -d "./bin" ]; then mkdir -p "./bin"; fi
