Desing Scratchpad
=================

#### What?

This is the "scratchpad" I'll use to record my ideas for how this XML parser will work.

Please also note that this project is not to create a complete fully compliant XML parser. It is almost entirely to be used to parse HTML.


Architecture
------------

I foresee an inheritance model somewhat like this:

- XmlNode
	- Element
	- TextElement
	- DoctypeElement

An `XmlNode` is the base type for all the other more specific nodes that can exist in an XML document.


### Attributes:

1. Children
	- "Children" is a vector of all the child XmlNodes
2. Parent
	- "Parent" is a reference to the parent XmlNode
3. Contents
	- "Contents" will only be used by the `TextElement` class, and will hold the actual contents of that node.

### Methods:



