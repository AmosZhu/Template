CC = g++
LIBS = 
SRCS += $(wildcard *.cpp)
INCLUDE_PATH += ../../include/ \
				../../CStack/ 

OBJS = $(patsubst %.cpp,%.o,$(SRCS))

CFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))
TARGET = StackTest


.PHONY:all
	all:$(TARGET)

$(TARGET):
	$(CC) -c -g $(CFLAGS) $(SRCS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf *.o
	rm -rf $(TARGET)
