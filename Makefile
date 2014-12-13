
BIN_DIR = ~/bin/
OUTPUT_NAME = link_grab
COMPILER = g++ -DDEBUG -g -std=c++11 -Wall

executable:
	$(COMPILER) main.cpp -lcurl -o $(OUTPUT_NAME)

clean:
	rm -fv $(OUTPUT_NAME)


all: executable

release: COMPILER = g++ -g -std=c++11
release: clean executable

install: release
	cp $(OUTPUT_NAME) ~/bin/

