#include <iostream>
#include <iterator>
#include <istream>
#include <ostream>
#include <string>
#include <map>

#include <unistd.h> /* isatty() */

#include "./html_parser/html_parser.cpp"
#include "./arg_parse/arg_parse.cpp"
#include "url_parse.cpp"

#ifndef NO_CURL
#define NO_CURL 0
#endif

#if NO_CURL
#else
#include "request.cpp"
#endif

// This function originally found here:
//     http://stackoverflow.com/a/202097
std::string read_stdin(){
	// don't skip whitespace while reading
	std::cin >> std::noskipws;

	// use stream iterators to copy the stream to a string
	std::istream_iterator<char> it(std::cin);
	std::istream_iterator<char> end;
	std::string result(it, end);

	return result;
}

int main(int argc, char const *argv[]){
	auto url = std::string("");
	auto args = argparse::ArgumentParser(argc, argv);

	// Use the argument parser to discern which url the user has specified.
	if (args.option_exists("url")){
		url = args.get_option("url");
	} else if (argc == 2){
		url = argv[1];
	} else {
		// Search for bare args that might be urls. The first one we find that
		// matches will be the one used.
		auto bare_args = args.get_bare_args();
		for (unsigned int i = 0; i < bare_args.size(); ++i){
			if (bare_args[i].find("//") != std::string::npos){
				url = bare_args[i];
				break;
			} else if (bare_args[i].find(".") != std::string::npos){
				url = bare_args[i];
				break;
			}
		}
	}
	if (url.empty() && argc > 1){
		std::cout << "Please enter a url." << std::endl;
		return 0;
	} else if (argc == 1){
		url = "stdin";
	}

	// Check if the user want's us to read from stdin.
	const char* page;
	if (args.option_exists("stdin")){
		page = read_stdin().c_str();
	} else if (url == "stdin"){
		page = read_stdin().c_str();
	} else {
#if NO_CURL
		std::cerr << 
		"Compiled without cURL support. This program cannot fetch pages on it's own.\n" << 
		"Pipe HTML into stdin to extract links." << std::endl;
		return 0;
#else
		page = handle_url(url.c_str());
#endif
	}

	std::string test_doc(page);

	htmlparser::HtmlParser tree(test_doc);
	std::vector<std::string> links = tree.find_all_attributes("href");

	// Keeps links from being printed multiple times
	std::map<std::string, std::string> unique_links;
	for (unsigned int i = 0; i < links.size(); ++i){
		unique_links[join_url(std::string(url), links[i])] = "";
	}
	for (auto it = unique_links.begin(); it != unique_links.cend(); ++it){
		std::cout << (*it).first << std::endl;
	}


	return 0;
}


