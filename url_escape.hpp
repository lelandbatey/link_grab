#ifndef URL_ESCAPE_HPP
#define URL_ESCAPE_HPP

#include <string>
#include <cctype> /* isalnum() */
#include <cstdio> /* sscanf() */
#include <sstream>
#include <iomanip> /* setw */

using namespace std;


// Due to a bug in the Cygwin GCC, the "to_string" function isn't available.
// So we implement out own!
//
// Taken from this stackoverflow post:
//     http://stackoverflow.com/a/20861692
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}



string encode_url(const string &value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << '%' << setw(2) << int((unsigned char) c);
    }

    return escaped.str();
}

// Decode's a given URL
//
// Taken from this stackoverflow post:
//     http://stackoverflow.com/a/4823686/
string decode_url(const string &in_url) {
	string ret;
	char ch;
	int tmp_char_as_int;
	for (size_t i = 0; i < in_url.length(); ++i) {
		if (int(in_url[i]) == 37) {
			sscanf(in_url.substr(i+1, 2).c_str(), "%x", &tmp_char_as_int);
			ch = static_cast<char>(tmp_char_as_int);
			ret += ch;
			i = i+2;
		} else {
			ret += in_url[i];
		}
	}
	return (ret);
}


#endif