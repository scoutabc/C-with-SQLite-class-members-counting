objects = main.o create.o create.h sqlite3.c help.o count.o change.o change_class.o change_student.o change_service.o

create.exe : $(objects)
	gcc -o create $(objects) -liconv

create.o : create.h sqlite3.c create.c
	gcc -c create.c
main.o : create.h sqlite3.c main.c
	gcc -c main.c
help.o : create.h sqlite3.c help.c
	gcc -c help.c
count.o : create.h sqlite3.c count.c
	gcc -c count.c
change.o : create.h sqlite3.c change.c
	gcc -c change.c
change_class.o : create.h sqlite3.c change_class.c
	gcc -c change_class.c
change_student.o : create.h sqlite3.c change_student.c
	gcc -c change_student.c
change_service.o : create.h sqlite3.c change_service.c
	gcc -c change_service.c

.PHONY : clean
clean :
	rm create $(objects)