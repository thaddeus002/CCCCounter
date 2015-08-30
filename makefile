# Top level makefile for CCCC

# This distribution is a compilation of code, some of which comes from
# different sources, some of which builds different (e.g. Win32 DLL) kinds
# of targets.
# I would like to make it less inconsistent, but the first stage is to make 
# it work...

DOX	= doxygen
CCCC	= ./cccc/cccc
CCCOPTS	= --lang=c++
CCCCSRC = ./cccc/*
GENSRC	= cccc/CLexer.cpp \
          cccc/CLexer.h \
          cccc/CParser.cpp \
          cccc/CParser.h \
          cccc/Ctokens.h \
          cccc/JLexer.cpp \
          cccc/JLexer.h \
          cccc/JParser.cpp \
          cccc/JParser.h \
          cccc/Jtokens.h \
          cccc/cccc.cpp \
          cccc/java.cpp \
          cccc/parser.dlg

.PHONY : all mini pccts cccc test

all : mini cccc test

mini :
	cd pccts && $(MAKE) -Orecurse antlr dlg || exit $$?

pccts :
	cd pccts && $(MAKE) -Orecurse $@ || exit $$?

cccc :	mini
	cd cccc && $(MAKE) -Orecurse -f posixgcc.mak $@ || exit $$?

.NOTPARALLEL:	test
test :
	cd test && $(MAKE) -Orecurse -f posix.mak test$@ || exit $$?

metrics :	cccc
	rm -rf metrics/*
	mkdir -p metrics
	$(CCCC) $(CCCOPTS) --outdir=./metrics/ $(CCCCSRC)

doxygen/html :	Doxyfile.html_cfg cccc/*.cc cccc/*.h
	rm -rf doxygen/html
	mkdir -p doxygen
	$(DOX) Doxyfile.html_cfg

docs :	Doxyfile.html_cfg doxygen/html

clean	:
	rm -rf cccc/*.o cccc/cccc $(GENSRC) pccts/bin/*

reallyclean :
	rm -rf metrics/* doxygen/html
