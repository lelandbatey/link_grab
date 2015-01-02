Link Scraper
============


Scrapes a page for all links, printing them to stdout. Great for batch downloading!

Prerequisites
-------------

You'll need to install the `libcurl` libraries. On Cygwin these are just called `libcurl`. On Debian (and derivatives) you'd install libcurl-dev (provided tested with GNU's: libcurl4-gnutls-dev).


Installation
------------

To compile the program, run `make release` in this directory. To install, run `make install` or copy the binary "link_grab" to wherever you'd like.

Usage
-----

Once compiled, use the program by passing in a link as a command line argument. It will download the specified web page and find all links in it, printing them out as it finds them.

