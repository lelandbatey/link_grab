
link_grab:
	g++ -std=c++11 main.cc -lcurl -o link_grab

all: link_grab

install: link_grab
	cp link_grab ~/bin/

clean:
	rm link_grab

