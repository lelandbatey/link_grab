#include <iostream>
#include <sstream>
#include <ctype.h>
#include <vector>
#include <string>
#include <memory>
#include <locale>
#include <map>

#include "xml_tree.h"


XmlTree::XmlTree(): _root(string("root")){}
XmlTree::XmlTree(string& input_document): _root(string("root")){
	build_xml_tree(input_document);
}



void XmlTree::build_xml_tree(string& input_document){
	istringstream doc(input_document);
	build_tree(&doc);
}

// Trampoline into the recursive search below
vector<string> XmlTree::find_all_attributes(string attribute_value){
	vector<string> found_attributes;

	find_attrs(attribute_value, &_root, &found_attributes);
	return found_attributes;
}

void XmlTree::find_attrs(string attr_val, XmlNode* node, vector<string>* found){

	if (node->has_attribute(attr_val)){
		found->push_back(node->get_attribute(attr_val));
	}

	if (node->has_children()){
		vector<XmlNode*> children = node->get_children();
		for (int i = 0; i < children.size(); ++i){
			find_attrs(attr_val, children[i], found);
		}
	}

}

// Macro for conveniently clearing all variables.
#ifndef _CLEAR_
#define _CLEAR_ state = ""; \
type = ""; \
tmp_value = ""; \
tmp_key = ""; \
tmp_type = ""; \
attributes.clear(); 
#endif

void XmlTree::build_tree(istringstream* input_document){
	istringstream& document = *input_document; 
	XmlNode* root = &_root;

	XmlNode* parent = root;
	map<string, string> attributes;
	string contents;
	string type;
	string tmp_key, tmp_value;
	string tmp_type;

	string buffer;

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

	while (!document.eof()){
		// Stands for "current character"
		char cc;
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
				if (cc == '!'){
					state = "doctype";
				} else {				
					state = "type";
					type.push_back(cc);
				}
			} else if (type.size() && isspace(cc)){
				state = "key";
			} else if (!type.size() && cc == '/'){
				state = "closing";
			} else if (cc == '>'){
				parent = new XmlNode(parent, type, attributes);
				_CLEAR_;
			}
		} else if (state == "doctype"){
			buffer.push_back(cc);
			if (buffer == "--"){
				state = "comment";
				buffer = "";
			}
			if (cc == '>'){
				state = "";
				buffer = "";
			}
		} else if (state == "comment") {
			if (cc == '-'){
				buffer.push_back(cc);
			} else if (cc == '>' && buffer == "--"){
				state = "";
				buffer = "";
			} else {
				if (isalnum(cc)){
					buffer = "";
				}
			}
		} else if (state == "key"){
			if (!isspace(cc) && cc != '=' && cc != '>'){
				// If there was a key but no value, and we're now looking at a second key.
				if (tmp_key.size() && attributes.count(tmp_key)){
					tmp_key = "";
				}
				tmp_key.push_back(cc);
			} else if (tmp_key.size() && cc == '='){
				state = "value";
			} else if (isspace(cc) && tmp_key.size()){
				attributes[strLower(tmp_key)] = "";
			} else if (cc == '>'){
				parent = new XmlNode(parent, type, attributes);
				_CLEAR_;
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
				attributes[strLower(tmp_key)] = tmp_value;
				tmp_value = "";
				tmp_key = "";
				state = "key";
			}
		} else if (state == "text"){
			if (cc != '<'){
				contents.push_back(cc);
			} else {
				// Prevents just whitespace from being made into a text node.
				for (int i = 0; i < contents.length(); ++i){
					if (!isspace(contents.c_str()[i])){
						new XmlNode(parent, string("text"), contents);
						break;
					}
				}

				contents = "";
				state = "opening";
			}
		} else if (state == "closing"){
			if (cc == '>'){
				XmlNode* tn = parent;
				while (tn->get_type() != tmp_type){
					if (tn->is_root()){
						// Force a segfault out of rage
						std::cout << "THIS XML DOCUMENT IS STUPID AND I HATE IT. WE'RE DONE!" << std::endl;
						tn = 0;
						tn->is_root();
					}
					tn = tn->get_parent();
				}

				parent = tn->get_parent();
				_CLEAR_;
			} else {

				tmp_type.push_back(cc);
			}
		}
	}
}


