CXX = /usr/bin/g++
CFLAGS = -Iinclude -I/usr/local/include -std=c++11 -O2
CLINKS = -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui

SRCDIR = src
OBJDIR = build

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJFILES = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCFILES))

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	CFLAGS += -fopenmp -Duse_openmp_
endif


all: wraper example


example: $(OBJFILES) example.cpp
	$(CXX) -o $@ $^ $(CFLAGS) $(CLINKS)


wraper: $(OBJFILES) python/wraper.cpp
	$(CXX) -o $(OBJDIR)/selective_search.so $^ $(CFLAGS) $(CLINKS) -shared -fPIC


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

clean:
	rm -f build/*.o build/*.so example
