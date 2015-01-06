#include "url_parse.hpp"
#include "lazy_string.hpp"
#include <string>
#include <vector>
#include <iostream>

// Attempts to parse a url.
// A url must be in one of threes formats (for now):
//
//     scheme://hostname/path
//     hostname/path
//     hostname
//
// There are many possible URL formats, but I'm going to ignore those (for
// now).
ParsedUrl::ParsedUrl(std::string begin_str):
	_scheme("http"), _host(""), _port(80), _path("/"), _fragment(""){ parse_url(begin_str); }


void ParsedUrl::parse_url(std::string inurl){
	// The behaviour of this function reproduces the behaviour of Python's
	// 'urllib.parse.urlsplit' function, which requires a scheme to recognize
	// a hostname. However, it has a special case for schemeless absolute
	// urls, which are specified with a '//'.
	
	size_t position = inurl.find("://");

	// Handle absolute links specially
	if (inurl.find("//") == 0){
		position = 2;
	}

	if (position && position != std::string::npos){
		_scheme = inurl.substr(0, position);
		
		// More special handling of absolute links
		if (_scheme == "//"){
			_scheme = "";
			position = -1;
		}
		// Remove the "host" demarcator from inurl
		inurl = inurl.substr(position + 3);
		position = inurl.find_first_of("/#");		
		
		if (inurl.size()){
			position = inurl.find_first_of("/");
			_host = inurl.substr(0, position);

			// Removes the port number
			_host = _host.substr(0, _host.find(":"));
		} else {
			position = 0;
		}
	} else {
		position = 0;
	}
	if (position < inurl.size()){
		inurl = inurl.substr(position);
	} else {
		inurl = "";
	}

	position = inurl.find("#");
	if (inurl.size()){
		position = position!=std::string::npos?position:inurl.size();
		_path = inurl.substr(0, position);
		_fragment = inurl.substr(position);
	}

	return;
}



std::string ParsedUrl::get_scheme(){ return _scheme; };
std::string ParsedUrl::get_host(){ return _host; }
int ParsedUrl::get_port(){ return _port; }
std::string ParsedUrl::get_path(){ return _path; }
std::string ParsedUrl::get_fragment(){ return _fragment; }

std::string ParsedUrl::get_url(){
	// Ternery if so we only include port if it's not 80
	return std::string(this->_scheme+"://"+this->_host
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

bool is_bare_fragment(std::string token){
	if (token[0] == '#'){
		return true;
	}
	return false;
}

std::string rejoin(std::vector<std::string>* v, char delim){
	std::string ret;
	for (unsigned int i = 0; i < v->size()-1; ++i){
		ret += (*v)[i];
		ret.push_back(delim);
	}
	ret += (*v)[v->size()-1];
	return ret;
}


std::string join_url(ParsedUrl first, ParsedUrl second){
	std::string scheme, host, fragment, path;

	if (second.get_scheme().size() && first.get_scheme() != second.get_scheme()){
		scheme = second.get_scheme();
	} else {
		scheme = first.get_scheme();
	}

	if (second.get_host().size() && first.get_host() != second.get_host()){
		host = second.get_host();
	} else {
		host = first.get_host();
	}
	if (second.get_fragment().size() && first.get_fragment() != second.get_fragment()){
		fragment = second.get_fragment();
	}

	if (second.get_path()[0] != '/'){
		path = first.get_path().substr(0, first.get_path().find_last_of('/'));
		path.push_back('/');
		path = path + second.get_path();
	} else {
		path = second.get_path();
	}

	return scheme + "://" + host + path + fragment;
}

std::string join_url(std::string first, std::string second){
	return join_url(ParsedUrl(first), ParsedUrl(second));
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

