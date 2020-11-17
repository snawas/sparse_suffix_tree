# sparse_suffix_tree
Implementation of a sparse suffix tree construction algorithm in C++

I wrote this implementation of a sparse suffix tree construction algorithm as part of a summer research project. Given the time constraints, there are unfinished portions of the algorithm such as querying the tree after it has been constructed. I intend to continue working on this project and will update it here as time permits.

Sparse suffix trees are a variation of the suffix tree data structure that are much faster to construct and traverse due to the decreased number of nodes in the tree. Partially based on the 2017 paper by Gawrychowski and Kociumaka, my sparse suffix tree algorithm employs optimal construction methods while maintaining reasonable cognitive breakdowns for readability.  

Gawrychowski and Kociumaka’s algorithm takes advantage of Karp-Rabin fingerprints as a centerpiece to speedier suffix tree construction. Breaking the text into “supercharacters” (fixed-size blocks of the text), the algorithm saves the fingerprint of each supercharacter which makes comparisons faster than brute-force checking individual characters. In doing so, the tree need not maintain lexicographical order during construction time, which saves on sorting costs. Once the general structure of the (unsorted) trie is formed, the algorithm switches the fingerprints back to the original text and sorts each level of the trie lexicographically to form the sparse suffix tree.
