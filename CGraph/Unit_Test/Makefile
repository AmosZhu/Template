CC = g++

CXXVERSION = $(shell g++ --version | grep ^g++ | sed 's/^.* //g')

LIBS =
SRCS += $(wildcard *.cpp)
INCLUDE_PATH += ../../CList/ \
				../../CQueue/ \
				../../CBinaryHeap/ \
				../../include/ \
				../../CGraph/

#PATCH
TEMP += $(wildcard ../*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
OBJS += $(TEMP:%.cpp=%.o)

ifeq "$(CXXVERSION)" "4.6.3"
	CFLAGS += -std=c++0x
else
	CFLAGS += -std=c++11
endif

CFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))
TARGET = GraphTest


.PHONY:all
	all:$(TARGET)

$(TARGET):
	cd .. && $(MAKE)
	$(CC) -c -g $(CFLAGS) $(SRCS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf *.o
	rm -rf $(TARGET)
	cd .. && make clean
