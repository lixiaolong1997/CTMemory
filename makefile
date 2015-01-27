SRCDIR = .
BUILDDIR = ./build

CDEBUG = -g -save-temps=obj
CINCLUDES =
CLIBS =
 
CXX = cc
CFLAGS = $(CDEBUG) $(CINCLUDES) $(CLIBS) -std=c99

default: before main after

before: clean

after:

clean:
	-test -d $(BUILDDIR) || mkdir -p $(BUILDDIR)
	-test -d $(BUILDDIR) && rm -rf $(BUILDDIR)/*
	-ctags -R
