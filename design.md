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


### Class Variables:

1. Children
	- "Children" is a vector of all the child XmlNodes
2. Parent
	- "Parent" is a reference to the parent XmlNode
3. Contents
	- "Contents" will only be used by the `TextElement` class, and will hold the actual contents of that node.
4. Attributes
	- "Attributes" is a map of the key-value pairs of the attributes on an element (style="color: blue;")
5. Type
	- "Type" contains the type of a node. For example an element `<i></i>` would have a type of `i`. 

### Methods:

- XmlNode(string type, map<string, string> attributes)
- has_children(): bool
- is_root(): bool
- has_text(): bool
- has_attribute(string): bool
- get_text(): string
- get_parent(): unique_ptr<XmlNode>
- get_attribute(string): string
- get_type(): string

