BIN_DIR  		:= bin
OBJ_DIR  		:= obj

SRC_DIR			:= src
INC_DIR			:= include

ROOTINC			:= $(shell root-config --incdir)
ROOTCFLAGS		:= $(shell root-config --cflags)
ROOTLIBS		:= $(shell root-config --libs)

CC 				:= g++ -std=c++11

LFLAGS			:= -Llib -L/usr/lib \
                	$(ROOTLIBS)
CFLAGS			:= -ggdb -fPIC -DLINUX -Wall -funsigned-char \
					-I$(INC_DIR) \
                	-I$(ROOTINC) -I$(ROOTCFLAGS) \
					-DCMAKE_CXX_FLAGS=-D_GLIBCXX_USE_CXX11_ABI=0

run: build
	bin/run

build: compile
	mkdir -p $(BIN_DIR)/
	g++ $(CFLAGS) \
		$(OBJ_DIR)/Main.o \
		$(OBJ_DIR)/FlatNTupleEngine.o \
		$(OBJ_DIR)/TrackBuilder.o \
		$(OBJ_DIR)/FlatNTuple.o \
		$(OBJ_DIR)/Plotter.o \
		-o $(BIN_DIR)/run \
        $(LFLAGS) -l curses

compile:
	mkdir -p $(OBJ_DIR)/
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Main.cpp -o $(OBJ_DIR)/Main.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)/FlatNTupleEngine.cpp -o $(OBJ_DIR)/FlatNTupleEngine.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)/TrackBuilder.cpp -o $(OBJ_DIR)/TrackBuilder.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)/FlatNTuple.cpp -o $(OBJ_DIR)/FlatNTuple.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Plotter.cpp -o $(OBJ_DIR)/Plotter.o

clean:
	rm -rf $(BIN_DIR)/
	rm -rf $(OBJ_DIR)/

remove:
	rm -rf $(BIN_DIR)/
	rm -rf $(OBJ_DIR)/

