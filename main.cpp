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
#include "print_help.cpp"

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
	auto read_from = std::string("");
	auto args = argparse::ArgumentParser(argc, argv);

	// Print our help message.
	if (args.option_exists("help") or args.bare_arg_exists("help")){
		print_help();
		return 0;
	}

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
		std::cout << "No url specified, exiting." << std::endl;
		return 0;
	}

	// Check if the user wants us to read from stdin or from a url
	if (args.option_exists("stdin") or args.bare_arg_exists("stdin")){
		read_from = "stdin";
	} else {
		read_from = "url";
	}

	const char* page;
	if (read_from == "stdin"){
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

	std::string document(page);

	htmlparser::HtmlParser tree(document);
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


