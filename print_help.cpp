#include <iostream>

void print_help(){
	std::cout << 
"link_grab, extracts links from web pages.\n"
"Usage:\n"
"    link_grab <url> [options]\n"
"    link_grab [options]\n"
"\n"
"Options:\n"
"    --help       Show this screen.\n"
"    --url=<url>  The url of the page to extract links from.\n"
"    --stdin      Read html from stdin. Requires the 'url' option be specified.\n"
"\n"
"Examples:\n"
"    link_grab example.com         Downloads html at example.com, prints extracted links.\n"
"    link_grab --url=example.com   Downloads html at example.com, prints extracted links.\n"
"    cat index.html | link_grab --stdin --url=example.com\n"
"                                  Reads html from stdin, interpreting the links as if\n"
"                                  they where links on 'example.com'.\n"
"    cat index.html | link_grab example.com --stdin\n"
"                                  Reads html from stdin, interpreting the links as if\n"
"                                  they where links on 'example.com'."
<< std::endl;
}