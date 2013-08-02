CC = g++
INCLUDE_PATH += ../
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=%.o)

CFLAGS = $(foreach dir,$(INCLUDE_PATH),-I$(dir))
CFLAGS += -g
TARGET = Matrix_Test

.PHONY:all
	all:$(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -c $(SRCS)
	$(CC) $(OBJS) -o $(TARGET)

clean:
	rm -rf *.o
	rm -rf $(TARGET)

