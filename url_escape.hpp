#ifndef URL_ESCAPE_HPP
#define URL_ESCAPE_HPP

#include <string>
#include <cctype> /* isalnum() */

using namespace std;

string escape_url(const string &in_url){
	string escaped;

	for (size_t i = 0; i < in_url.size(); ++i){
		char c = in_url[i]; 
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~'){
			escaped.push_back(c);
			continue;
		}
		escaped.push_back('%');
		escaped.append(to_string(int((unsigned char) c)));
	}
	return escaped;
}




#endif