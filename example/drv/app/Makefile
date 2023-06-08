
SPIRIT_DIR=../../../definitions

CCPP=g++
CPPFLAGS=-Wall
CPPFLAGS+=-I$(SPIRIT_DIR)
LIBS=-pthread
BUILD_DIR=build

SRC=drvtalk.cpp
APP=drvtalk

all: 
	$(CCPP) $(SRC) $(CPPFLAGS) $(LIBS) -o $(APP)

.PHONY:clean
clean:
	rm -f *.o $(APP)
