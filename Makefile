
BIN_DIR = ~/bin/
OUTPUT_NAME = link_grab
COMPILER = g++ -DDEBUG -g -std=c++11

clean:
	rm -fv $(OUTPUT_NAME)

executable:
	$(COMPILER) main.cc -lcurl -o $(OUTPUT_NAME)

all: executable

release: COMPILER = g++ -g -std=c++11
release: clean executable

install: release
	cp $(OUTPUT_NAME) ~/bin/

