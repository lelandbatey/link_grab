#include <iostream>
#include <string>
#include <map>
#include "./html_parser/html_parser.cpp"
#include "request.cc"
#include "url_parse.cc"


int main(int argc, char const *argv[]){
    std::string url;
    if (argc > 1){
        url = argv[1];
    } else {
        std::cout << "Please enter a url." << std::endl;
        return 0;
    }

    char* page = handle_url(url.c_str());

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


