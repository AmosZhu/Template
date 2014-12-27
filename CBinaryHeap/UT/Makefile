CC = g++
CXXVERSION = $(shell g++ --version | grep ^g++ | sed 's/^.* //g')

LIBS =
SRCS += $(wildcard *.cpp)
ROOTPATH = ../../..

INCLUDE_PATH += $(ROOTPATH)/Template/include/ \
				$(ROOTPATH)/Template/CBinaryHeap/ \

OBJS = $(patsubst %.cpp,%.o,$(SRCS))

CFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))

ifeq "$(CXXVERSION)" "4.6.3"
	CFLAGS += -std=c++0x
else
	CFLAGS += -std=c++11
endif

TARGET = BinaryHeap

.PHONY:all
	all:$(TARGET)

$(TARGET):
	$(CC) -c -g $(CFLAGS) $(SRCS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf *.o
	rm -rf $(TARGET)
