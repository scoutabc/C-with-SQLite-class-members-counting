object = create.o

create:$(object)
	gcc -lm -o create $(object) -lsqlite3

create.o:create.c
	gcc -c create.c -lsqlite3