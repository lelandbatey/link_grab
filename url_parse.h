#ifndef URL_PARSE_H
#define URL_PARSE_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>



using namespace std;


// Url object takes a string that's supposed to be a URL (uniform resource
// locator) and parse it into it's appropriate parts. Then we can do things
// like combine URL's if we want to.
//
// This is going to somewhat replicate the functionality of Python's
// `urlparse` library, though I won't parse parameters or queries.
class ParsedUrl
{
public:
	ParsedUrl(string begin_str);

	friend ostream& operator<< (ostream &out, Point &cPoint);
	string get_scheme();
	string get_host();
	string get_port();
	string get_path();

	string get_url();

private:
	string _scheme;
	string _host;
	string _port;
	string _path;
};


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
string join_url(string host, string path);



#endif