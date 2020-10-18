#ifndef SNODE_H
#define SNODE_H
#include <map>

class snode
{
public:
    typedef std::map<std::string, snode*> map;

    // default constructor
    snode()
    {
        _children = map();
        _location = -1;
        _length = 0;
    }

    // constructor
    snode(long long location, long long length) : _location(location), _length(length)
    {
        _children = map();
    }

    // destructor
    ~snode()
    {
        for(auto e: _children)
            delete e.second;
    }


    // pre: none
    // post: returns pointer to the descendant snode of this snode with edge label key
    const snode* get_child(std::string key) const
    {
        return _children.at(key);
    }

    // pre: none
    // post: sets the snode child as a descendant of this snode with edge label key
    void set_child(std::string key, snode* child)
    {
        _children[key] = child;
    }

    // pre: none
    // post: returns the map of children of this snode
    map& get_children()
    {
        return _children;
    }

    // pre: none
    // post: returns the index of the first character of the substring represented by this snode
    long long get_location() const
    {
        return _location;
    }

    // pre: none
    // post: returns the length of the substring represented by this snode
    long long get_length() const
    {
        return _length;
    }

private:
    map _children;
    long long _location;
    long long _length;

};

#endif // SNODE_H
