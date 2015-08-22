# Compiler
CC = g++
OPTS = -fPIC -std=c++14 -O3 -g0 -Wall -D_REENTRANT -I./json/

# Project name
PROJECT = server
LIBNAME = libcppserver

# Directories
OBJDIR = obj
SRCDIR = .

# Libraries
LIBS = -lpthread
LDFLAGS = -shared

# Files and folders
SRCS    = $(shell find $(SRCDIR) -name '*.cpp')
SRCDIRS = $(shell find . -name '*.cpp' | dirname {} | sort | uniq | sed 's/\/$(SRCDIR)//g' )
OBJS    = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Targets
$(PROJECT): buildrepo $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(OPTS) -c $< -o $@
	
clean:
	rm $(PROJECT) $(OBJDIR) $(LIBNAME).so -Rf

lib:
	$(CC) $(OBJS) $(LIBS) $(LDFLAGS) -o $(LIBNAME).so
	
buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef