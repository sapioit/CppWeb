# Compiler
CXX = clang++
OPTS = -fPIC -std=c++14 -O3 -Wall -D_REENTRANT
DEBUGOPTS = -g
INCLUDEDIRS = -I.

# Project name
PROJECT = srv

# Libraries
LIBS = -lpthread
LDFLAGS = -shared
LIBNAME = libcppweb.so
LIBNAME_ABBREVIATED = -lcppweb

# Files and folders
#include main.o back
OBJS    = cachemanager.o parser.o request.o response.o responsemanager.o routeutility.o filesystem.o \
	outputscheduler.o socket.o watcher.o jsoncpp.o dfa.o log.o resource.o settings.o storage.o dispatcher.o \
	server.o
TOBJS	= main.o

# Targets
$(PROJECT): $(OBJS) $(TOBJS)
	$(CXX) $(OPTS) $(DEBUGOPTS) $(LIBS) $^ -o $@

%.o : %.cpp
	$(CXX) -c $(OPTS) $(DEBUGOPTS) $(INCLUDEDIRS) $< -o $@
	
%.o : http/%.cpp
	$(CXX) -c $(OPTS) $(DEBUGOPTS) $(INCLUDEDIRS) $< -o $@

%.o : io/%.cpp
	$(CXX) -c $(OPTS) $(DEBUGOPTS) $(INCLUDEDIRS) $< -o $@
	
%.o : json/%.cpp
	$(CXX) -c $(OPTS) $(DEBUGOPTS) $(INCLUDEDIRS) $< -o $@
	
%.o : misc/%.cpp
	$(CXX) -c $(OPTS) $(DEBUGOPTS) $(INCLUDEDIRS) $< -o $@
	
%.o : server/%.cpp
	$(CXX) -c $(OPTS) $(DEBUGOPTS) $(INCLUDEDIRS) $< -o $@

clean:
	rm *.o $(PROJECT) $(LIBNAME) -Rf

lib: $(OBJS)
	$(CXX) $(OPTS) $(DEBUGOPTS) $(LIBS) $(LDFLAGS) $^ -o $(LIBNAME)

testapp: main.o
	$(CXX) $(OPTS) $(DEBUGOPTS) $(LIBS) -L./ $(LIBNAME_ABBREVIATED) $^ -o $(PROJECT)

install: 
	cp $(LIBNAME) /usr/lib
