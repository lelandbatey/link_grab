#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>

using namespace std;


class XmlNode
{
public:
	XmlNode(string type);
	XmlNode(string type, map<string, string> attributes);
	XmlNode(std::unique_ptr<XmlNode> parent, string type, map<string, string> attributes);

	bool has_children();
	bool is_root();
	bool has_text();
	bool has_attribute(string key);

	string get_text();
	unique_ptr<XmlNode> get_parent();
	string get_attribute(string key);
	string get_type();

	
private:
	string _type;
	map<string, string> _attributes;
	vector<XmlNode> _children;
	string _contents;
	unique_ptr<XmlNode> _parent;

};


unique_ptr<XmlNode> build_xml_tree(const string& file_name);
unique_ptr<XmlNode> build_xml_tree(string input_document);


