# source files.
SRC = box.cpp sa3d.cpp container.cpp mpeg4_container.cpp
OBJ = $(SRC:.cpp=.o)
OUT = libspatialmedia.a
 
# include directories
INCLUDES = -I. -I../include/ -I/usr/local/include
 
# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g
 
# compiler
CCC = g++
 
# library paths
LIBS = -L../ -L/usr/local/lib -lm
 
# compile flags
LDFLAGS = -g
 
.SUFFIXES: .cpp
 
# d e f a ult: dep $(OUT)

default: $(OUT)
 
.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@
 
$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)
 
depend: dep
 
dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)
 
clean:
	rm -f $(OBJ) $(OUT) Makefile.bak 

