#include <iostream>
#include <string>
#include "xml_node.cc"
#include "request.cc"
#include "url_parse.cc"
#include "xml_tree.cc"

using namespace std;

int main(int argc, char const *argv[]){
    string url;
    if (argc > 1){
        url = argv[1];
    } else {
        std::cout << "Please enter a url." << std::endl;
        return 0;
    }

    char* page = handle_url(url.c_str());

    string test_doc(page);
    XmlTree tree(test_doc);
    vector<string> links = tree.find_all_attributes("href");

    for (unsigned int i = 0; i < links.size(); ++i){
        std::cout << join_url(string(url), links[i]) << std::endl;
    }

    return 0;
}


