
SPIRIT_DIR=../..

CCPP=g++
CPPFLAGS=-Wall
CPPFLAGS+=-I$(SPIRIT_DIR)/definitions -I$(SPIRIT_DIR)/tools/app
LIBS=-pthread -lrt
BUILD_DIR=build

SRC=apptalk.cpp $(SPIRIT_DIR)/tools/app/ipcmqlib.cpp
APP=apptalk

all: 
	$(CCPP) $(SRC) $(CPPFLAGS) $(LIBS) -o $(APP)

.PHONY:clean
clean:
	rm -f *.o $(APP)
