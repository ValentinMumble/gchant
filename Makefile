# Build GChant with gtkmm 2.4 (on a Mac: brew install gtkmm pkgconf)

CXX ?= c++
CXXFLAGS ?= -O2 -std=c++11
GTKMM_CFLAGS := $(shell pkg-config gtkmm-2.4 --cflags)
GTKMM_LIBS := $(shell pkg-config gtkmm-2.4 --libs)

SOURCES = main.cpp gchant.cpp dxfarea.cpp dxfhandler.cpp \
	src/objects.cpp src/dxflib/dl_dxf.cpp src/dxflib/dl_writer_ascii.cpp
OBJECTS = $(SOURCES:.cpp=.o)

GChant: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(GTKMM_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(GTKMM_CFLAGS) -I src -c $< -o $@

clean:
	rm -f $(OBJECTS) GChant

.PHONY: clean
