# Top level makefile for CCCC

# Programms
AR ?= ar
CPP ?= cpp -E
CC ?= gcc
CCC ?= g++
LD ?= g++
DOX = doxygen

# Options
CCCOPTS	= --lang=c++



# Targets

.PHONY : all cccc test

all : cccc

cccc :
	$(MAKE) DEBUG=$(DEBUG) -C src $@ || exit $$?

.NOTPARALLEL: cccc test
test : cccc
	$(MAKE) -C test || exit $$?

DOCS	= doxygen
METRICS	= ccccout
TESTOBJ = test/*.xml test/*.html test/*.db

$(METRICS)/.keep_dir :
	mkdir -p $(dir $@)
	touch $@

metrics : $(METRICS)/.keep_dir cccc
	rm -rf $(METRICS)/*.html
	@echo "Input files are: $(CCCCSRC)"
	$(CCCC) $(CCCOPTS) --outdir=$(METRICS)/ $(CCCCSRC)
	@echo "Metrics output now in $(METRICS)/cccc.html"

$(DOCS)/.keep_dir :
	mkdir -p $(dir $@)
	touch $@

docs :	Doxyfile.html_cfg $(CCCCSRC) $(DOCS)/.keep_dir
	rm -rf $(DOCS)/html
	$(DOX) Doxyfile.html_cfg
	@echo "API docs now in $(DOCS)/html"

clean :
	$(MAKE) -C src clean

mrproper:
	$(MAKE) -C src mrproper
	$(MAKE) -C test mrproper

install :
	install src/cccc /usr/local/bin/
	@echo ===========================
	@echo Installation succeeded!
	@echo ===========================

reallyclean : clean
	rm -rf ccccout/* doxygen/html test/.cccc $(TESTOBJ)


