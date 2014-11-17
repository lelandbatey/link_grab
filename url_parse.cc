#include "url_parse.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>



using namespace std;
// Given a string that is a host






// Two functions for splitting a string. The first uses a pre-constructed
// vector, the second returns a new vector.
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

string join(std::vector<string> v, char delim){
	string ret;
	for (int i = 0; i < v.size()-1; ++i){
		ret += v[i];
		ret.push_back(delim);
	}
	ret += v[v.size()-1];
	return ret;
}

// Prints out a vector of strings
void print_vect(std::vector<string> v){
	int i;
	for (i = 0; i < v.size(); ++i){
		std::cout << "'" << v[i] << "', ";
	}
	std::cout << v.size() << std::endl;
	// std::cout << v[v.size()-1] << std::endl;
}

string join_url(string host, string path){
	vector<string> spl_host = split(host, '/');
	print_vect(spl_host);
	std::cout << join(split(host, '/'), '/') << std::endl;


	return string("whut");
}



int main(int argc, char const *argv[]){
	string h("http://thing");
	string p("what/stuff/goes");

	join_url(h, p);
	return 0;
}
