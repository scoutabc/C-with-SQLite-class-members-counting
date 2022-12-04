objects = main.o create.o create.h sqlite3.c

create : $(objects)
	cc -o create $(objects)

create.o : create.h sqlite3.c
	cc -c create.c create.o
main.o : create.h sqlite3.c
	cc -c main.c main.o

.PHONY : clean
clean :
	rm create main.o create.o