
SPIRIT_DIR=../..

CC=gcc
CCFLAGS=-Wall
CXX=g++
CXXFLAGS=-Wall
CXXFLAGS+=-I$(SPIRIT_DIR)/definitions -I$(SPIRIT_DIR)/tools/app
CXXLIBS=-pthread -lrt
BUILD_DIR=build

SRC=apptalk.cpp $(SPIRIT_DIR)/tools/app/ipcmqlib.cpp
APP=apptalk

all: 
	$(CXX) $(SRC) $(CXXFLAGS) $(LIBS) -o $(APP)

.PHONY:clean
clean:
	rm -f *.o $(APP)
