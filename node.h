#ifndef NODE_H
#define NODE_H
#include <unordered_map>
#include "InfInt.h"

class node
{
public:
    // the print of a node is the fingerprint of the string it represents
    // the <string> field of the children map is the "edge" from the parent node to the child
    // the "edge" int is the fingerprint for the supercharacter that represents the prefix of the child's "print" field
    // the location of the node is the index in _text where the string was first found (repeats do not overwrite this value)
    // the length of the node is the length of the string the print represents

    typedef std::unordered_map<std::string, node*> map;

    // default constructor
    node()
    {
        _print = 0;
        _children = map();
        _location = -1;
        _length = 0;
    }

    // constructor
    node(InfInt print, long long location, long long length) : _print(print), _location(location), _length(length)
    {
        _children = map();
    }

    // destructor
    ~node()
    {
        for(auto e: _children)
            delete e.second;
    }

    // pre: none
    // post: returns pointer to the descendant node stored under the key edge label
    node* get_child(std::string key) const
    {
        return _children.at(key);
    }

    // pre: none
    // post: assigns node child to be a desecendant of this node with edge label key
    void set_child(std::string key, node* child)
    {
        _children[key] = child;
    }

    // pre: none
    // post: return the full map of children of this node
    map& get_children()
    {
        return _children;
    }

    // pre: none
    // post: sets the entire map of children of this node to the specified map
    void set_children(map& children)
    {
        _children = children;
    }

    // pre: none
    // post: sets the entire map of children of this node to the empty map
    void clear_children()
    {
        _children = map();
    }

    // pre: none
    // post: returns the index of the first character of the substring represented by this node
    long long get_location() const
    {
        return _location;
    }

    // pre: none
    // post: returns the length of the substring represented by this node
    long long get_length() const
    {
        return _length;
    }

    // pre: none
    // post: sets the length of the substring represented by this node to l
    void set_length(long long l)
    {
        _length = l;
    }

    // pre: none
    // post: returns the fingerprint of the substring represented by this node
    const InfInt& get_print() const
    {
        return _print;
    }

    // pre: none
    // post: sets the fingerprint of the substring represented by this node
    void set_print(InfInt p)
    {
        _print = p;
    }

private:
    InfInt _print;
    map _children;
    long long _location;
    long long _length;

};
#endif // NODE_H
