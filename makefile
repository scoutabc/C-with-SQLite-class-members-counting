objects = main.o create.o create.h sqlite3.c help.o

create.exe : $(objects)
	gcc -o create $(objects) -liconv

create.o : create.h sqlite3.c create.c
	gcc -c create.c
main.o : create.h sqlite3.c main.c
	gcc -c main.c
help.o : create.h sqlite3.c help.c
	gcc -c help.c

.PHONY : clean
clean :
	rm create main.o create.o