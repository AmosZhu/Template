INCLUDE = -I../include/
CC = g++
OBJECTS = CList.o

.PHONY:obj
	obj:CList.o

CList.o:CList.hpp
	$(CC) $(INCLUDE) -c CList.hpp -o $(OBJECTS)


clean:
	rm -f *.o

