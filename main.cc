#include <iostream>
#include <string>
#include "parser.cc"
#include "request.cc"

using namespace std;

int main(int argc, char const *argv[]){
    string url;
    if (argc > 1){
        url = argv[1];
    } else {
        std::cout << "Please enter a url." << std::endl;
        return 0;
    }

	// string test_doc("<HTML><HEAD><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"><TITLE>301 Moved</TITLE></HEAD></HTML>");

    char* page = handle_url(url.c_str());

    string test_doc(page);

	XmlNode* parsed_doc = build_xml_tree_from_string(test_doc);
    print_href(parsed_doc);
    // print_xml_tree(parsed_doc);

    return 0;
}


