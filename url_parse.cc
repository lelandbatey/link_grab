#include "url_parse.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <string.h>


using namespace std;
// Given a string that is a host

// Attempts to parse a url.
// A url must be in one of threes formats (for now):
//
//     scheme://hostname/path
//     hostname/path
//     hostname
//
// There are many possible URL formats, but I'm just going to ignore those
// (for now).
ParsedUrl::ParsedUrl(string begin_str):
	_scheme("http"), _host(""), _port(80), _path("/") 
{
	ParseStr input(begin_str);

	// std::cout << begin_str << std::endl;

	string buffer("");
	string state("");
	string scheme_buf, host_buff, path_buf;

	while (!input.eof()){
		char cc;
		cc = input.get();

		if (state == ""){
			if (cc == ':'){
				state = "scheme";
				scheme_buf = buffer;
				buffer = "";
			} else if (cc == '/'){
				state = "path";
				host_buff = buffer;
				scheme_buf = "http";
				buffer = "";
				buffer.push_back(cc);
			} else {
				buffer.push_back(cc);
			}
		} else if (state == "scheme"){
			buffer.push_back(cc);
			if (buffer == "//"){
				state = "host";
				buffer = "";
			}
		} else if (state == "host"){
			if (cc == '/'){
				host_buff = buffer;
				buffer = "";
				state = "path";
			}
			buffer.push_back(cc);
		} else if (state == "path"){
			buffer.push_back(cc);
		}
	}
	if (state == "path"){
		path_buf = buffer;
	} else if (state == "host"){
		host_buff = buffer;
	} else if (state == ""){
		host_buff = buffer;
	}

	if (scheme_buf.length()){
		_scheme = scheme_buf;
	}
	if (host_buff.length()){
		_host = host_buff;
	}
	if (path_buf.length()){
		_path = path_buf;
	}
	// std::cout << "Scheme: '" << scheme_buf << "'"<< std::endl;
	// std::cout << "Host: '" << host_buff << "'"<< std::endl;
	// std::cout << "Path: '" << path_buf << "'"<< std::endl;

}

string ParsedUrl::get_scheme(){ return _scheme; };
string ParsedUrl::get_host(){ return _host; }
int ParsedUrl::get_port(){ return _port; }
string ParsedUrl::get_path(){ return _path; }

string ParsedUrl::get_url(){
	// Ternery if so we only include port if it's not 80
	return string(this->_scheme+"://"+this->_host
		+ (this->_port != 80?":"+dubToStr(double(this->_port)):"")
		+ this->_path);
}
ostream& operator<< (ostream &out, ParsedUrl &pUrl){
	out << pUrl.get_url();
	return out;
}

void ParsedUrl::set_path(string path){

	// If a path is a url
	if (path.find("://") != std::string::npos){
		ParsedUrl purl(path);
		path = purl.get_path();
	}

	std::vector<string> spl_path = split(path, '/');
	string p("");

	for (int i = 0; i < spl_path.size(); ++i){
		if (spl_path[i].length()){
			p += "/"+spl_path[i];
		}
	}
	_path = p;
}

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

string rejoin(std::vector<string> v, char delim){
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
	ParsedUrl h(host);

	h.set_path(path);

	return h.get_url();
}



// int main(int argc, char const *argv[]){
// 	// string h("http://thing");
// 	// string p("what/stuff/goes");

// 	// join_url(h, p);
// 	ParsedUrl url("lelandbatey.com/posts/2014/09/binary-tree-printer/");
// 	ParsedUrl url2("/thing.html");

// 	// std::cout << url2 << std::endl;

// 	url.set_path("thing.html");
// 	std::cout << url << std::endl;
// 	return 0;
// }
