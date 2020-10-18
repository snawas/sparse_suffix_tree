#ifndef TRIE_H
#define TRIE_H
#include <unordered_map>
#include "phis.h"
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include "InfInt.h"
#include "node.h"

class trie
{
public:
    // the trie structure is nodes whose branches are stored in the map of children

    typedef std::unordered_map<std::string, node*> map;

    // constructor
    trie(std::string *text, long long n, InfInt p, InfInt x, long long rounds) : _fingerprints(text, n, p, x, rounds), _text(text),
                                                                            _n(n), _p(p), _x(x), _rounds(rounds), _lce(0)
    {
        _root = new node();
        construct();
        std::cout << "LCEs: " << _lce << std::endl;
    }

    // destructor
    ~trie()
    {
        clear_nodes(_root);
    }

    // pre: none
    // post: inserts suffix starting at i into the trie and returns the index of the next suffix to be inserted
    long long insert(long long i)
    {
        // start searching for supercharacter matches at the root
        node* u = _root;

        while(i < _n)
        {
            // sc_print is the fingerprint of the first supercharacter of the string we wish to insert into the trie
            InfInt sc_print = calculate_fingerprint(i, i+_rounds);
            std::string sc_print_string = sc_print.toString();

            // check if the suffixes start with this supercharacter or not
            if(u->get_children().count(sc_print_string)==0)
            {
                // if the parent node has no matching children, create a new node for this supercharacter
                node* temp = new node(0, i, _n - i);

                // if there are characters left over, then the label is fingerprint of the entire suffix
                if(i + _rounds < _n)
                    temp->set_print(_fingerprints.get_fingerprint(i));
                else
                    temp->set_print(sc_print);

                // insert this new node with the edge label being the fingerprint of the first supercharacter in the substring
                u->set_child(sc_print_string, temp);
                return i+_rounds;
            }
            else
            {
                // if we found a match, then we follow this path down the trie
                // v is the child of u that matched the supercharacter we were looking for
                node* v = u->get_child(sc_print_string);

                // check if edge label is the same as the node label
                // if not, then the node represents more text than just one supercharacter
                if(sc_print != v->get_print())
                {
                    int match = LCE(i, v->get_location());

                    // round down to a supercharacter multiple
                    match = match - match%_rounds;

                    // split the node into separate branches
                    node* temp = new node(0, v->get_location() + match, v->get_length() - match);
                    temp->set_children(v->get_children());
                    temp->set_print(calculate_fingerprint(temp->get_location(), temp->get_location() + temp->get_length()));

                    // resetting the branch node and its children
                    v->clear_children();
                    v->set_child(calculate_fingerprint(temp->get_location(), temp->get_location() + _rounds).toString(), temp);
                    v->set_print(calculate_fingerprint(v->get_location(), v->get_location() + match));
                    v->set_length(match);

                    i = i + match;
                    node* temp2 = new node(_fingerprints.get_fingerprint(i), i, _n - i);
                    v->set_child(calculate_fingerprint(i, i+_rounds).toString(), temp2);
                    return i;
                }
                else
                {
                    // otherwise, simply follow the path down the trie with the matching supercharacters
                    u = v;
                    i = i + _rounds;
                }
            }
        }
        // return -1 if there is a failure to insert
        return -1;
    }

    // pre: none
    // post: returns a pointer to the root node of this trie
    node* get_root()
    {
        return _root;
    }

private:
    node* _root;
    phis _fingerprints;
    std::string * _text;
    long long _n;
    InfInt _p;
    InfInt _x;
    long long _rounds;
    long long _lce;

    // pre: none
    // post: constructs the coarse compacted trie according to the algorithm
    void construct()
    {
        long long temp;
        for(long long i=0; i<_n;)
        {
            temp = insert(i);

            // if temp = -1, there was a failure to insert the previous node into the trie
            assert(temp!=-1);
            i = temp;
        }
    }

    // pre: none
    // post: deletes children of parent node and recursively deletes children of those children
    void clear_nodes(node* parent)
    {
        for(auto e: parent->get_children())
            clear_nodes(e.second);
        parent->get_children().clear();
        delete parent;
    }

    // pre: none
    // post: returns the fingerprint of _text[index..end-1]
    InfInt calculate_fingerprint(long long index, long long end)
    {
        assert(index < _n);
        InfInt suffix2;
        if (end >= _n)
            suffix2 = 0;
        else
            suffix2 = _fingerprints.get_fingerprint(end);
        InfInt suffix1 = _fingerprints.get_fingerprint(index);
        InfInt exp = fast_exponentiation(_x, end-index, _p);
        InfInt calculation = (suffix1 - ((exp*suffix2)%_p))%_p;

        // C++ modulo returns a negative number if the 1st operand is negative -> we want the positive modulo
        if(calculation < 0)
            calculation = calculation + _p;
        return calculation;
    }

    // pre: none
    // post: calculates the Longest Common Extension of the suffixes at index i and j
    long long LCE(long long i, long long j)
    {
        assert(i < _n && j < _n);
        long long t = 0;
        while((_text->at(i+t)==_text->at(j+t)) && ((j+t)<=_n))
            t++;
        _lce++;
        return t;
    }

    // pre: none
    // post: prints the trie structure
    void print(node* parent)
    {
        for(auto i : parent->get_children())
        {
            std::cout << "Parent: " << parent->get_print() << std::endl;
            std::cout << "Edge: " << i.first << "   Print: " << i.second->get_print() << std::endl;
            for(auto j : i.second->get_children())
                print(i.second);
        }
        std::cout << "Number of Children: " << parent->get_children().size() << std::endl;
        std::cout << std::endl;
    }

    // pre: none
    // post: calculates a^m (mod n) and returns the result as an InfInt
    InfInt fast_exponentiation(InfInt a, InfInt m, InfInt n)
    {
        InfInt ans;
        if(m%2==1)
            ans = a;
        else
            ans = 1;
        m = m/2;
        while(m>0)
        {
            a = (a*a)%n;
            if(m%2==1)
            {
                if(ans==1)
                    ans = a;
                else
                    ans = (ans*a)%n;
            }
            m = m/2;
        }
        return ans;
    }
};

#endif // TRIE_H
