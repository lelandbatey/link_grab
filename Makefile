
BIN_DIR = ~/bin/
OUTPUT_NAME = link_grab
COMPILER = g++ -g -std=c++11
LIBS = -lcurl
GIT_UPDATE_SUBMODULE = git submodule init && git submodule update && git submodule foreach git pull origin master

# Tests if cURL is installed, and if not, compiles the program without it.
CURL_TEST:=$(shell echo '\#include <curl/curl.h>' | g++ -E -xc++ - > /dev/null 2>&1; echo $$? )
ifneq ($(CURL_TEST),0)
	COMPILER+= -DNO_CURL
	LIBS=
endif
ifneq ($(CURL_TEST),0)
$(info cURL not installed. Program will be compiled without cURL support. HTML must be piped via STDIN to be interpreted.)
endif
ifeq ($(CURL_TEST),0)
$(info cURL library installed.)
endif


all: executable

html_parser/html_parser.cpp:
	$(GIT_UPDATE_SUBMODULE)

arg_parse/arg_parse.cpp:
	$(GIT_UPDATE_SUBMODULE)

executable: html_parser/html_parser.cpp arg_parse/arg_parse.cpp
	$(COMPILER) main.cpp $(LIBS) -o $(OUTPUT_NAME)


clean:
	rm -fv $(OUTPUT_NAME)

debug: COMPILER+= -DDEBUG
debug: clean executable

nocurl: COMPILER+= -DNO_CURL
nocurl: clean executable

release: clean executable

install: release
	cp $(OUTPUT_NAME) ~/bin/

