#ifndef PARSE_STR_H
#define PARSE_STR_H
#include <string>
#include <iostream>

using namespace std;


// A wrapper around a string to allow for easy retrieval of single characters.
class ParseStr
{
public:
	ParseStr(string input):_str(input), _pos(0), _eof(false){};

	char get(){
		char cc;
		if (_pos < _str.size()){
			cc = _str[_pos];
			++_pos;
		} else {
			cc = '\0';
			_eof = true;
		}
		return cc;
	}

	bool eof(){
		if (_pos == _str.size()){
			_eof = true;
		}
		return _eof;
	}


private:
	string _str;
	int _pos;
	bool _eof;
	
};

std::string dubToStr(double dub) {
    std::ostringstream temp;

    temp << dub;
    
    std::string result(temp.str());
    return result; 
}

#endif