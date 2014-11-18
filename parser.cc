#include <iostream>
#include <sstream>
#include <ctype.h>
#include <vector>
#include <string>
#include <memory>
#include <locale>
#include <algorithm>
#include <map>

#include "parser.h"
#include "parse_str.h"

#pragma once

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
	if (_contents.length()){
		return true;
	}
	return false;
}

bool XmlNode::has_attribute(string key){
	key = strLower(key);
	if (_attributes.count(key) > 0){
		return true;
	}
	return false;
}

string XmlNode::get_text(){ return _contents; }
XmlNode* XmlNode::get_parent(){ return _parent; }
vector<XmlNode*> XmlNode::get_children(){ return _children; }
string XmlNode::get_type(){ return _type; }
string XmlNode::get_attribute(string key){
	key = strLower(key);
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

XmlNode::XmlNode(string type):_parent(0), _type(strLower(type)){}

XmlNode::XmlNode(string type, map<string, string> attributes):
	_parent(0), _type(strLower(type)), _attributes(attributes){}

XmlNode::XmlNode(XmlNode* parent, string type, string contents):
	_parent(parent), _type(strLower(type)), _contents(contents){

	_parent->add_child(this);
}

XmlNode::XmlNode(XmlNode* parent, string type, map<string, string> attributes):
	_type(strLower(type)), _attributes(attributes), _parent(parent){
	_parent->add_child(this);
}

void xml_tree_printer(XmlNode* node){
	for (int i = 0; i < node->get_depth(); ++i){
		std::cout << "    ";
	}
	if (node->has_text()){
		std::cout << "'" << node->get_text() << "'" << std::endl;
	} else {
		std::cout << "<" << node->get_type() << ">" << std::endl;
	}

	if (node->has_children()){
		vector<XmlNode*> children = node->get_children();
		for (int i = 0; i < children.size(); ++i){
			xml_tree_printer(children[i]);
		}
	}

	if (!node->has_text()){
		for (int i = 0; i < node->get_depth(); ++i){
			std::cout << "    ";
		}
		std::cout << "</" << node->get_type() << ">" << std::endl;
		
	}

}


void print_xml_tree(XmlNode* root){
	xml_tree_printer(root);
}


void print_href(XmlNode* node){
	string key("href");
	if (node->has_attribute(key)){
		std::cout << node->get_attribute(key) << std::endl;
	}
	if (node->has_children()){
		vector<XmlNode*> children = node->get_children();
		for (int i = 0; i < children.size(); ++i){
			print_href(children[i]);
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

XmlNode* build_xml_tree(istringstream* input_document){
	// istringstream document(input_document);
	istringstream& document = *input_document; 
	XmlNode* root = new XmlNode(string("root"));

	XmlNode* parent = root;
	map<string, string> attributes;
	string contents;
	string type;
	string tmp_key, tmp_value;
	string tmp_type;


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
			if (cc == '>'){
				state = "";
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
						std::cout << parent->get_type() << std::endl;
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
	return root;
}

XmlNode* build_xml_tree_from_string(string input_document){
	istringstream doc(input_document);
	return build_xml_tree(&doc);
}

