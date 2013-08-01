CC = g++
SRCS += $(wildcard *.cpp)
INCLUDE_PATH += ../CList/ \
				../CQueue/ \
				../include/

OBJS := $(SRCS:.cpp=.o)

RM = rm -rf

CFLAGS += $(foreach dir,$(INCLUDE_PATH), -I$(dir))

.PHONY:all

all:
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	$(RM) $(OBJS)

