CC = g++
LIBS =
SRCS += $(wildcard *.cpp)
ROOTPATH = ../../..

INCLUDE_PATH += $(ROOTPATH)/Template/include/ \
				$(ROOTPATH)/Template/CLeftHeap/ \
				$(ROOTPATH)/Template/CStack/ \
				$(ROOTPATH)/Template/CQueue/

OBJS = $(patsubst %.cpp,%.o,$(SRCS))

CFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))
TARGET = LeftHeap

.PHONY:all
	all:$(TARGET)

$(TARGET):
	$(CC) -c -g $(CFLAGS) $(SRCS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf *.o
	rm -rf $(TARGET)
