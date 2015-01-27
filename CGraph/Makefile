CC = g++

CXXVERSION = $(shell g++ --version | grep ^g++ | sed 's/^.* //g')

SRCS += $(wildcard *.cpp)
INCLUDE_PATH += ../CList/ \
				../CQueue/ \
				../include/

OBJS := $(SRCS:.cpp=.o)

RM = rm -rf

CFLAGS += $(foreach dir,$(INCLUDE_PATH), -I$(dir))

ifeq "$(CXXVERSION)" "4.6.3"
	CFLAGS += -std=c++0x
else
	CFLAGS += -std=c++11
endif

.PHONY:all

all:
	$(CC) $(CFLAGS) -c -g $(SRCS)

clean:
	$(RM) $(OBJS)

