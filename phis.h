#ifndef PHIS_H
#define PHIS_H
#include <vector>
#include <unordered_map>
#include <math.h>
#include <cassert>
#include <iostream>
#include "InfInt.h"

class phis
{
public:

    typedef std::unordered_map<long long, InfInt> map;

    // constructor
    phis(std::string* text, long long n, InfInt p, InfInt x, long long rounds)
    {
        _n = n;
        _p = p;
        _x = x;
        _rounds = rounds;
        _text = text;
        compute();
    }

    // pre: index is not out of bounds
    // post: returns fingerprint of suffix starting at index
    InfInt get_fingerprint(long long index)
    {
        assert(index < _n);
        return _phis[index%_rounds].at(index);
    }

    // pre: none
    // post: returns fingerprint of the entire text
    InfInt get_text_fingerprint()
    {
        return _fingerprint;
    }

private:
    std::vector<map> _phis;
    long long _n;
    InfInt _p;
    InfInt _x;
    long long _rounds;
    std::string* _text;
    InfInt _fingerprint;

    // pre: none
    // post: initializes _phis with phi sets for all r <= rounds
    void compute()
    {
        // initialize the maps (one for each round)
        for(long long i=0; i<_rounds; ++i)
        {
            map phi;
            _phis.push_back(phi);
        }

        // calculate the initial fingerprint (the suffix of _text length 1)
        int v = _text->at(_n-1);
        InfInt val = v;
        InfInt prev = val%_p;
        _phis[(_n-1)%_rounds][_n-1] = prev;

        // parse through the text backwards to fill fingerprints in _phis
        for(long long i=_n-2; i>=0; i--)
        {
            v = _text->at(i);
            InfInt val2 = v;
            InfInt mult = (prev*_x)%_p;
            prev = (mult + val2%_p)%_p;
            _phis[i%_rounds][i] = prev;
        }
        _fingerprint = prev;
    }
};



#endif // PHIS_H
