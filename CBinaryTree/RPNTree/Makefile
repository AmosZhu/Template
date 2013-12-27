CC = g++
LIBS = 
SRCS += $(wildcard *.cpp)
ROOTPATH = ../../..
RPNPATH=$(ROOTPATH)/DataStructure/Chapter3/20ReversePolishNotation

INCLUDE_PATH += $(ROOTPATH)/Template/include/ \
				$(ROOTPATH)/Template/CBinaryTree/   \
				$(ROOTPATH)/Template/CStack/	\
				$(RPNPATH)/

OBJS = $(patsubst %.cpp,%.o,$(SRCS))
OBJS += $(RPNPATH)/RPNCalculate.o

CFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))
TARGET = PRNTree


.PHONY:all
	all:$(TARGET)

$(TARGET):
	cd ../../../DataStructure/Chapter3/20ReversePolishNotation/ && $(MAKE)
	$(CC) -c -g $(CFLAGS) $(SRCS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf *.o
	rm -rf $(TARGET)
