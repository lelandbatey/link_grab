#include "url_parse.h"
#include "lazy_string.h"
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
// There are many possible URL formats, but I'm going to ignore those (for
// now).
ParsedUrl::ParsedUrl(string begin_str):
	_scheme("http"), _host(""), _port(80), _path("/"), _fragment(""){ parse_url(begin_str); }


void ParsedUrl::parse_url(string inurl){
	// std::string scheme("http"), host, path("/"), fragment;

	int position = inurl.find("://");
	if (position && position != std::string::npos){
		_scheme = inurl.substr(0, position);
		// Remove the "host" demarcator from inurl
		inurl = inurl.substr(position + 3);
		// std::cout << "_Scheme:   " << _scheme << " " << position << std::endl;
	} else {
		position = 0;
	}

	position = inurl.find_first_of("/#");
	if (inurl.size()){
		position = position!=std::string::npos?position:inurl.size();
		_host = inurl.substr(0, position);
		// Removes the port number, if there is one.
		_host = _host.substr(0, _host.find(":"));
		// std::cout << "_host:     " << _host << " " << position << std::endl;
	} else {
		position = 0;
	}
	inurl = inurl.substr(position);

	position = inurl.find("#");
	if (inurl.size()){
		position = position!=std::string::npos?position:inurl.size();
		_path = inurl.substr(0, position);
		_fragment = inurl.substr(position);
		// std::cout << "_path:     " << _path << " " << position << std::endl;
		// std::cout << "_fragment: " << _fragment << std::endl;
	}

	return;
}



string ParsedUrl::get_scheme(){ return _scheme; };
string ParsedUrl::get_host(){ return _host; }
int ParsedUrl::get_port(){ return _port; }
string ParsedUrl::get_path(){ return _path; }
string ParsedUrl::get_fragment(){ return _fragment; }

string ParsedUrl::get_url(){
	// Ternery if so we only include port if it's not 80
	return string(this->_scheme+"://"+this->_host
		+ (this->_port != 80?":"+dubToStr(double(this->_port)):"")
		+ this->_path + this->_fragment);
}

ostream& operator<< (ostream &out, ParsedUrl &pUrl){
	out << pUrl.get_url();
	return out;
}

ParsedUrl& ParsedUrl::operator=(const ParsedUrl& a){
	_scheme = a._scheme;
	_host = a._host;
	_port = a._port;
	_path = a._path;
	return *this;
}

bool is_bare_fragment(string token){
	if (token[0] == '#'){
		return true;
	}
	return false;
}

string rejoin(std::vector<string>* v, char delim){
	string ret;
	for (unsigned int i = 0; i < v->size()-1; ++i){
		ret += (*v)[i];
		ret.push_back(delim);
	}
	ret += (*v)[v->size()-1];
	return ret;
}

std::vector<std::string> tokenize_url(){

};

std::vector<std::string> tokenize_path(std::string path_token){
	std::vector<std::string> tokens;
	std::string buffer("");

	for (unsigned int i = 0; i < path_token.size(); ++i){
		char cc = path_token[i];
		if (cc == '/' || cc == '#'){
			// Last logic check ensures no repeated delimiters
			if (buffer.size() && buffer.back() != cc){
				tokens.push_back(buffer);
				buffer = "";
				buffer.push_back(cc);
			}
		} else {
			buffer.push_back(cc);
		}
	}
	if (buffer.size()){
		if (buffer == "/"){
			tokens.back().push_back('/');
		} else {
			tokens.push_back(buffer);
		}
	}
	return tokens;
}

// std::string join_relative_url(std::string host, std::string guest){
// 	std::vector<std::string> host_token = tokenize_path(host);
// 	std::vector<std::string> guest_token = ;


// }

void ParsedUrl::set_path(string token){

	// Saves us from combining paths.
	if (token.find("://") != std::string::npos){
		this->parse_url(token);
		return;
	}

	string p("");

	std::vector<string> split_token = split(token, '/');
	if (is_bare_fragment(token)){
		_fragment = token;
	// For absolute token, overwrite entire path
	} else if (token[0] == '/'){
		for (unsigned int i = 0; i < split_token.size(); ++i){
			if (split_token[i].length()){
				p += "/"+split_token[i];
			}
		}
		_path = p;
	// Join relative paths
	} else {
		std::vector<std::string> split_path = split(_path, '/');
		split_path[split_path.size()-1] = split_token[0];
		for (unsigned int i = 1; i < split_token.size(); ++i){

			split_path.push_back(split_token[i]);
		}
		p = rejoin(&split_path, '/');
		// p.insert(0, "/");
		_path = p;
	}
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


// Prints out a vector of strings
void print_vect(std::vector<string> v){

	for (unsigned int i = 0; i < v.size(); ++i){
		std::cout << "'" << v[i] << "', ";
	}
	std::cout << v.size() << std::endl;
}

string join_url(string host, string path){
	ParsedUrl h(host);
	std::cout << "Path:                   " << path << std::endl;
	std::cout << "Host:                   " << host << std::endl;
	std::cout << "Parsed host:            " << h.get_url() << std::endl << std::endl;

	std::cout << h.get_path() << std::endl;
	std::cout << h.get_fragment() << std::endl;

	h.set_path(path);
	std::cout << "Combined host and path: " << h.get_url() << std::endl;

	return h.get_url();
}

