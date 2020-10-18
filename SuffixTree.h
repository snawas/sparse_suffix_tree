#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H
#include "trie.h"
#include <map>
#include "snode.h"


class SuffixTree
{
public:

    // constructor
    SuffixTree(std::string *text, long long n, InfInt p, InfInt x, long long rounds) : _text(text), _p(p), _nodes(0)
    {
        // the suffix tree uses the trie object for its own construction
        _t = new trie(text, n, p, x, rounds);
        _root = new snode();
        fill_tree(_t->get_root(), _root);

        // delete the trie once we are done referencing it to free up memory
        delete _t;
    }

    // destructor
    ~SuffixTree()
    {
        clear_nodes(_root);
    }

    // pre: none
    // post: prints a list of children at each level of the suffix tree, with the root being level 0
    void print(snode* parent, int level)
    {
        std::cout << "Level: " << level << std::endl;
        for(auto e: parent->get_children())
        {
            std::cout << "child: " << e.first << std::endl;
            if(!e.second->get_children().empty())
                print(e.second, level+1);
        }
        std::cout << std::endl;
    }

    // pre: none
    // post: returns a pointer to the root of this suffix tree
    snode* get_root()
    {
        return _root;
    }

    // pre: none
    // post: returns whether or not this suffix tree has a simple structure (meaning none of the children nodes of the root have children)
    bool simple()
    {
        for(auto e: _root->get_children())
            if(!(e.second->get_children().empty()))
                return false;
        return true;
    }

    // pre: none
    // post: returns the number of nodes in this sparse suffix tree
    long long get_num_nodes()
    {
        return _nodes;
    }

private:
    trie* _t;
    std::string* _text;
    InfInt _p;
    snode* _root;
    long long _nodes;

    // pre: none
    // post: frees up memory allocated for all the nodes in this suffix tree
    void clear_nodes(snode* parent)
    {
        for(auto e: parent->get_children())
            clear_nodes(e.second);
        parent->get_children().clear();
        delete parent;
    }

    // pre: none
    // post: using the reference nodes from the trie, this function constructs the sparse suffix tree by inserting the snodes
    //       in the same structure as the trie but with the children of each node sorted lexicographically
    void fill_tree(node* ref, snode* match)
    {
        // loop through the children of the reference node in the trie
        for(auto e: ref->get_children())
        {
            // count the number of nodes being created
            _nodes++;

            // create a new suffix tree node with the same information as the respective trie node
            snode* temp = new snode(e.second->get_location(), e.second->get_length());
            std::string label = _text->substr(temp->get_location(), temp->get_length());
            match->set_child(label, temp);

            // recursively call this function on the children of these nodes in order to parse through the entire trie
            fill_tree(e.second, temp);
        }
    }

};
#endif // SUFFIXTREE_H
