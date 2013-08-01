INCLUDE = -I../include/
CC = g++

.PHONY:obj
	obj:CQueue.o

CQueue.o:CQueue.hpp
	$(CC) $(INCLUDE) -c CQueue.hpp -o $@

clean:
	rm -f *.o
