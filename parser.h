#include <stdio>
#include <vector>
#include <string>

using namespace std;


class XmlNode
{
public:
	XmlNode();
	
private:
	vector<XmlNode> children;
	string contents;
	

};

