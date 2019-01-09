CXX = /usr/bin/g++

# incoporate your opencv `include` and `lib` here
OPENCV_INCLUDE = -I/usr/local/include
OPENCV_LIBRARY = -L/usr/local/lib

CFLAGS = $(OPENCV_INCLUDE) -Iinclude -std=c++11 -O2 -fPIC
CLINKS = $(OPENCV_LIBRARY) -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui

SRCDIR = src
OBJDIR = build

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJFILES = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCFILES))

# Clang in MacOS may not support openmp
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	CFLAGS += -fopenmp -Duse_openmp_
endif


all: wraper example


example: $(OBJFILES) example.cpp
	$(CXX) -o $@ $^ $(CFLAGS) $(CLINKS)


wraper: $(OBJFILES) python/wraper.cpp
	$(CXX) -o $(OBJDIR)/selective_search.so $^ $(CFLAGS) $(CLINKS) -shared


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

clean:
	rm -f build/*.o build/*.so example
