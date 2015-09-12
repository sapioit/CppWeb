# Compiler
CC = clang++
OPTS = -fPIC -std=c++14 -O3 -g -Wall -D_REENTRANT -march=native
DEBUGOPTS = 
INCLUDEDIRS = -I.

# Project name
PROJECT = srv

# Libraries
LIBS = -lpthread
LDFLAGS = -shared

# Files and folders
OBJS    = main.o cachemanager.o parser.o request.o response.o responsemanager.o routeutility.o filesystem.o \
	outputscheduler.o socket.o watcher.o jsoncpp.o dfa.o log.o resource.o settings.o storage.o dispatcher.o \
	server.o

# Targets
$(PROJECT): $(OBJS)
	$(CC) $(OPTS) $(DEBUGOPTS) $(LIBS) $^ -o $@

%.o : %.cpp
	$(CC) -c $(OPTS) $(DEBUGOPTS) $(INCLUDEDIRS) $< -o $@
	
%.o : http/%.cpp
	$(CC) -c $(OPTS) $(DEBUGOPTS) $(INCLUDEDIRS) $< -o $@

%.o : io/%.cpp
	$(CC) -c $(OPTS) $(DEBUGOPTS) $(INCLUDEDIRS) $< -o $@
	
%.o : json/%.cpp
	$(CC) -c $(OPTS) $(DEBUGOPTS) $(INCLUDEDIRS) $< -o $@
	
%.o : misc/%.cpp
	$(CC) -c $(OPTS) $(DEBUGOPTS) $(INCLUDEDIRS) $< -o $@
	
%.o : server/%.cpp
	$(CC) -c $(OPTS) $(DEBUGOPTS) $(INCLUDEDIRS) $< -o $@

clean:
	rm *.o $(PROJECT) -Rf

lib:
	$(CC) $(OBJS) $(LIBS) $(LDFLAGS) -o $(LIBNAME).so
