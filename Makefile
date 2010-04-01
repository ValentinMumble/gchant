
prefix =	/usr/local
exec_prefix =	${prefix}
bindir =	${exec_prefix}/bin

OBJF =		main.o \
            dxfhandler.o \
			gchant.o \
			dxfarea.o
EXTERN_LIBS = -L ./lib -ldxflib -L/usr/X11R6/lib -lX11
INCLUDES = -I ./src

all: main

clean:
	@echo "Cleaning project..."
	@find . -name '*.o' -exec rm "{}" \;
	@if [ -f GChant ]; then rm -f GChant; fi

distclean: clean
	rm -f config.log config.cache Makefile config.status

main: $(OBJF)
	g++ -o GChant $(OBJF) $(EXTERN_LIBS) ./lib/objects.o `pkg-config gtkmm-2.4 --cflags --libs`

%.o : %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) -O3 -Wall $< -o $@ `pkg-config gtkmm-2.4 --cflags --libs`

doc :
	@echo "Building documentation"
	@doxygen doxy.cfg
