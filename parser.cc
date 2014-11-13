#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <map>

#include "parser.h"

using namespace std;

bool XmlNode::has_children(){
	if (_children.empty()){
		return true;
	}
	return false;
}

bool XmlNode::is_root(){
	if (!_parent){
		return true;
	}
	return false;
}

bool XmlNode::has_text(){
	if (_contents.empt()){
		return true;
	}
	return false;
}

string XmlNode::get_text(){ return _contents; }
unique_ptr<XmlNode> XmlNode::get_parent(){ return _parent; }
string XmlNode::get_type(){ return _type; }
string XmlNode::get_attribute(string key){
	if (_attributes.count(key) > 0){
		return _attributes[key];
	} else {
		return string("");
	}
}

XmlNode::XmlNode(string type){
	_type = type;
}

XmlNode::XmlNode(string type, map<string, string> attributes){
	_type = type;
	_attributes = attributes;
}

XmlNode::XmlNode(unique_ptr<XmlNode> parent, string type, map<string, string> attributes){
	_type = type;
	_attributes = attributes;
	_parent = parent;
}


unique_ptr<XmlNode> build_xml_tree(string input_document){
	istringstream document(input_document);
	XmlNode root(string("root"));

	unique_ptr<XmlNode> parent = root;
	map<string, string> attributes;
	string contents;
	string type;
	// Stands for "current character"
	char cc;

	// State will be our flag for which part of the parsing process we are
	// currently doing. Possible states are:
	//
	// 1. Opening tag
	//     - Type
	//     - Attributes
	// 2. Closing tag
	// 3. Doctype tag
	// 4. Text value
	// 5. Comment
	// 6. Unknown
	string state("");

	while (!document.eof()){
		cc = document.get();
		if (states.empty()){
			if (cc == '<'){
				states = "opening";
			}
		}

	}

}


// int main(int argc, char const *argv[]){
	
// 	return 0;
// }


