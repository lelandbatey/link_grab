#include <iostream>
#include <sstream>
#include <ctype.h>
#include <vector>
#include <string>
#include <memory>
#include <map>

#include "parser.h"

using namespace std;

bool XmlNode::has_children(){
	if (!_children.empty()){
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
	if (_contents.empty()){
		return true;
	}
	return false;
}

string XmlNode::get_text(){ return _contents; }
XmlNode* XmlNode::get_parent(){ return _parent; }
vector<XmlNode*> XmlNode::get_children(){ return _children; }
string XmlNode::get_type(){ return _type; }
string XmlNode::get_attribute(string key){
	if (_attributes.count(key) > 0){
		return _attributes[key];
	} else {
		return string("");
	}
}
int XmlNode::get_depth(){
	if (!is_root()){
		return _parent->get_depth()+1;
	} else {
		return 0;
	}
}

void XmlNode::add_child(XmlNode* child){
	_children.push_back(child);
}

XmlNode::XmlNode(string type):_parent(0){
	_type = type;
}

XmlNode::XmlNode(string type, map<string, string> attributes):_parent(0){
	_type = type;
	_attributes = attributes;
}

XmlNode::XmlNode(XmlNode* parent, string type, string contents):_parent(0){
	_type = type;
	_contents = contents;
	_parent = parent;
	_parent->add_child(this);
}

XmlNode::XmlNode(XmlNode* parent, string type, map<string, string> attributes):_parent(0){
	_type = type;
	_attributes = attributes;
	_parent = parent;
	_parent->add_child(this);
}

void xml_tree_printer(XmlNode* node){
	for (int i = 0; i < node->get_depth(); ++i){
		std::cout << "\t";
	}
	std::cout << "<" << node->get_type() << ">" << std::endl;
	if (node->has_children()){
		vector<XmlNode*> children = node->get_children();
		for (int i = 0; i < children.size(); ++i){
			xml_tree_printer(children[i]);
		}
	}

}

void print_xml_tree(XmlNode* root){
	xml_tree_printer(root);
}



XmlNode* build_xml_tree(istringstream* input_document){
	// istringstream document(input_document);
	istringstream& document = *input_document; 
	XmlNode* root = new XmlNode(string("root"));

	XmlNode* parent = root;
	map<string, string> attributes;
	string contents;
	string type;
	string tmp_key, tmp_value;
	// Stands for "current character"
	char cc;

	// State will be our flag for which part of the parsing process we are
	// currently doing. Possible states are:
	//
	// 1. Opening tag
	//     - Type
	//     - Attributes
	//         - Key
	//         - Value
	// 2. Closing tag
	// 3. Doctype tag
	// 4. Text value
	// 5. Comment
	// 6. Unknown
	string state("");

	std::cout << cc << ", '" << state << "'" << std::endl;
	while (!document.eof()){
		cc = document.get();

		if (document.eof()){
			continue;
		}

		if (state.empty()){
			if (cc == '<'){
				state = "opening";
			} else {
				state = "text";
				contents.push_back(cc);
			}
		} else if (state == "opening" || state == "type"){
			if (!isspace(cc) && cc != '>' && cc != '/'){
				state = "type";
				type.push_back(cc);
			} else if (type.size() && isspace(cc)){
				state = "key";
			} else if (!type.size() && cc == '/'){
				state = "closing";
			} else if (cc == '>'){
				state = "";
				print_xml_tree(root);
				parent = new XmlNode(parent, type, attributes);
				type = "";
				tmp_value = "";
				tmp_key = "";
			}
		} else if (state == "key"){
			if (!isspace(cc) && cc != '='){
				// If there was a key but no value, and we're now looking at a second key.
				if (tmp_key.size() && attributes.count(tmp_key)){
					tmp_key = "";
				}
				tmp_key.push_back(cc);
			} else if (tmp_key.size() && cc == '='){
				state = "value";
			} else if (isspace(cc) && tmp_key.size()){
				attributes[tmp_key] = "";
			} else if (cc == '>'){
				state = "";
				print_xml_tree(root);
				parent = new XmlNode(parent, type, attributes);
				type = "";
				tmp_value = "";
				tmp_key = "";
			}
		} else if (state == "value"){
			// We get the value of things in quotes
			char delim = 0;
			if (cc == '"' || cc == '\''){
				delim = cc;
			}
			if (delim){
				bool escaped = false;
				cc = document.get();

				while (escaped || cc != delim){
					if (!escaped && cc == '\\'){
						escaped = true;
					} else {
						tmp_value.push_back(cc);
						escaped = false;
					}
					cc = document.get();
				}
				attributes[tmp_key] = tmp_value;
				tmp_value = "";
				tmp_key = "";
				state = "key";
			}
		} else if (state == "text"){
			if (cc != '<'){
				contents.push_back(cc);
			} else {
				new XmlNode(parent, string("text"), contents);
				contents = "";
				state = "opening";
			}
		} else if (state == "closing"){
			if (cc == '>'){
				parent = parent->get_parent();
				state = "";
			}
		}
		std::cout << cc << ", '" << state << "'" << std::endl;
	}
	return root;
}

XmlNode* build_xml_tree_from_string(string input_document){
	istringstream doc(input_document);
	return build_xml_tree(&doc);
}

