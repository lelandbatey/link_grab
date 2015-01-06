
BIN_DIR = ~/bin/
OUTPUT_NAME = link_grab
COMPILER = g++ -g -std=c++11

all: executable

html_parser/html_parser.cpp:
	git submodule init && git submodule update && git submodule foreach git pull origin master

executable: html_parser/html_parser.cpp
	$(COMPILER) main.cpp -lcurl -o $(OUTPUT_NAME)

clean:
	rm -fv $(OUTPUT_NAME)

debug: COMPILER = g++ -DDEBUG -g -std=c++11 -Wall
debug: clean executable

release: clean executable

install: release
	cp $(OUTPUT_NAME) ~/bin/

