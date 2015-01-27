CC = g++
CXXVERSION = $(shell g++ --version | grep ^g++ | sed 's/^.* //g')

INCLUDE_PATH += ../../include/\
				../../CList/

SRCS += $(wildcard *.cpp)
OBJS += $(SRCS:%.cpp=%.o)
CFLAGS += $(foreach dir,$(INCLUDE_PATH),-I$(dir))

ifeq "$(CXXVERSION)" "4.6.3"
	CFLAGS += -std=c++0x
else
	CFLAGS += -std=c++11
endif

TARGET = CList_Test

.PHONY:all
	all:$(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -c -g $(SRCS)
	$(CC) $(OBJS) -o $(TARGET)

clean:
	rm -rf *.o
	rm -rf $(TARGET)


