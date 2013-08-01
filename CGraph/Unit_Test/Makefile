CC = g++
LIBS = 
SRCS += $(wildcard *.cpp)
INCLUDE_PATH += ../../CList/ \
				../../CQueue/ \
				../../include/ \
				../../CGraph/ 

#PATCH
TEMP += $(wildcard ../*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
OBJS += $(TEMP:%.cpp=%.o)

CFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))
TARGET = GraphTest


.PHONY:all
	all:$(TARGET)

$(TARGET):
	cd .. && $(MAKE)
	$(CC) -c $(CFLAGS) $(SRCS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf *.o
	rm -rf $(TARGET)
	cd .. && make clean
