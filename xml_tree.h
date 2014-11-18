#include <iostream>
#include <vector>
#include <string>

#include "parser.cc"

using namespace std;

class XmlTree
{
public:
	XmlTree();
	XmlTree(string& input_document);
	
	void build_xml_tree(string& input_document);
	void print_xml_tree();

	vector<string> find_all_attributes(string attribute_value);

	// ~XmlTree();

private:
	XmlNode _root;
	void build_tree(istringstream* input_document);
	void find_attrs(string attr_val, XmlNode* node, vector<string>* found);
};

