CC = g++
INCLUDE_PATH += ../../include/\
				../../CList/

SRCS += $(wildcard *.cpp)
OBJS += $(SRCS:%.cpp=%.o)
CFLAGS += $(foreach dir,$(INCLUDE_PATH),-I$(dir))
TARGET = CList_Test

.PHONY:all
	all:$(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -c -g $(SRCS)
	$(CC) $(OBJS) -o $(TARGET)

clean:
	rm -rf *.o
	rm -rf $(TARGET)


