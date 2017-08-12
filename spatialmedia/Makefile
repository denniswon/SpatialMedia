appname := spatialmedia

CXX := g++
CCFLAGS  := -ggdb3 -O3
CFLAGS := -I. -I./mpeg -I./third_party/mxml-2.10
CXXFLAGS := -ggdb3 -O0 -I./third_party/mxml-2.10
LDFLAGS  += ./third_party/mxml-2.10/libmxml.a ./mpeg/libspatialmedia.a -lpthread
# LIBS := -lmxml

srcfiles := $(shell ls -1 *.cpp)
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: mxml MPEG $(appname)

MPEG:
	if [ ! -e ./mpeg/libspatialmedia.a ]; then cd mpeg; make; cd ..; fi

mxml:
	if [ ! -e ./third_party/mxml-2.10/libmxml.so.1.5 ]; then cd third_party/mxml-2.10; ./configure; make; cd ../../; fi

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) -o $(appname) $(objects) $(LDLIBS) $(LDFLAGS)

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -f $(objects) $(appname)
	cd ./mpeg; make clean
	cd ./third_party/mxml-2.10; make clean 

dist-clean: clean
	rm -f *~ .depend

include .depend
