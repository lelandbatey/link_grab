#include <iostream>
#include <string>
#include "parser.cc"

using namespace std;

int main(int argc, char const *argv[]){
	string test_doc("<html><h1>Boy I hope this works!</h1></html>");

	XmlNode* parsed_doc = build_xml_tree_from_string(test_doc);

	std::cout << parsed_doc->has_children() << std::endl;
	std::cout << parsed_doc->get_children().size() << std::endl;;


	print_xml_tree(parsed_doc);
	return 0;
}


