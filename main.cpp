#include <iostream>
#include <vector>
#include <unordered_map>
#include "phis.h"
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>
#include "trie.h"
#include "InfInt.h"
#include "SuffixTree.h"
#include <fstream>

using namespace std;

auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
mt19937 generator(seed);

// fills the text array with random ints
string* randomize_text(long long n, int alphabet_size)
{
    string t = "";
    char c;
    uniform_int_distribution<int> distribution(1, alphabet_size);
    for (long long i=0; i < n-1; ++i)
    {
        c = distribution(generator);
        t = t+c;
    }
    c = 0;
    t = t+c;               // end the string with a character outside of the alphabet (e.g. '$')
    string* ans = new string(t);
    return ans;
}

// Fast Exponentiation: http://homepages.math.uic.edu/~leon/cs-mcs401-s08/handouts/fastexp.pdf
// calculates a^m (mod n)
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

// Rabin-Miller Primality Test
bool is_prime(InfInt n)
{
    if(n>2 && n%2==0)
        return false;
    InfInt s = 1;
    InfInt m;
    bool found = false;
    InfInt exp(1);

    // want to find m, s such that n − 1 = m * 2^s, with m odd and s ≥ 1
    while(!found)
    {
        // calculate 2^s
        exp = 1;
        for(InfInt i(1); i<s; ++i)
            exp = exp * 2;

        if(((n-1)%exp)==0)           // check if (n-1)/2^s is an integer
        {
            m = (n-1)/exp;
            if(m%2 == 1)             // check if m is odd
                found = true;
        }
        ++s;
    }

    // choose a random int a such that 0 < a < n
    long long limit;
    if(n > LONG_LONG_MAX)
        limit = LONG_LONG_MAX;
    else
        limit = n.toLongLong() - 1;
    uniform_int_distribution<long long> distribution(2, limit);
    long long a = distribution(generator);

    // calculate a^m and check (mod n)
    InfInt index = 1;
    InfInt check = fast_exponentiation(a, m, n);
    if(check%n == 1 || check%n == (n-1))
        return true;

    while(index<s)
    {
        check = fast_exponentiation(check, 2, n);       // square the previous result for efficiency
        if(check%n == 1 || check%n == (n-1))
            return true;
        index++;
    }

    // if none of the checks work out, we determine that n is likely composite
    return false;
}

// utility function to run Rabin-Miller a few times for certainty
bool verify_prime(InfInt n)
{
    for(int i=0; i<5; ++i)
        if(!is_prime(n))
            return false;
    return true;
}

// checks random numbers between num and approx 2*num for a prime
InfInt suggest_prime(InfInt num)
{
    // Sieve of Eratosthenes
    int n = 5000;
    vector<bool> is_prime(n, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= n; i++)
        if (is_prime[i] && (long long)i * i <= n)
            for (int j = i * i; j <= n; j += i)
                is_prime[j] = false;

    // store all the found prime numbers less than 5000 in a vector
    vector<int> primes;
    for(int i=2; i<n; ++i)
        if(is_prime[i])
            primes.push_back(i);

    // create a random number one digit at a time
    int digits = num.numberOfDigits();
    if(num.digitAt(0) >= 4)
        digits++;
    uniform_int_distribution<int> ds(0, 9);
    bool found = false;
    string create = "";
    InfInt check;
    while(!found)
    {
        create = "";
        for(int i=0; i<digits; ++i)
            create = create + to_string(ds(generator));
        check = create;
        if(check > num)
        {
            // check divisibility by the primes we calculated
            bool pass = true;
            for(size_t i=0; i<primes.size(); ++i)
                if(check%primes[i] == 0)
                {
                    pass = false;
                    break;
                }
            if(pass)
            {
                // then check Rabin-Miller primality test
                found = verify_prime(check);
            }
        }
    }
    return check;
}

int main()
{
    //auto start = chronho::high_resolution_clock::now();

    long long n = 32000;             // user input
    long long b = 16000;           // user input
    int alphabet_size = 128;        // user input
    double c = 1.0;               // user input

    // select prime p such that p ≥ max(|∑|, n^(3+c))
    InfInt p = 1;
    for(int i=0; i<3+c; ++i)
        p = p*n;
    p = suggest_prime(p);

    long long limit;
    if(p > LONG_LONG_MAX)
        limit = LONG_LONG_MAX;
    else
        limit = p.toLongLong() - 1;

    uniform_int_distribution<long long> distribution(1, limit);
    InfInt x = distribution(generator);

    string* text;
    string choice = "random";

    if(choice=="random")
    {
        text = randomize_text(n, alphabet_size);
    }
    else
    {
        ifstream in("bookcrossing.txt");
        string t;
        string s;
        if (in.is_open())
        {
          while (getline(in, s))
          {
              t += s;
          }
          in.close();
        }
        else
            cout << "Unable to open file";

        char ascii = 0;
        t = t + ascii;
        if((size_t)n > t.length())
            n = t.length();
        cout << "n = " << n << endl;
        text = new string(t);
    }

    long long rounds = ceil((double)n/(double)b);
    cout << "Rounds: " << rounds << endl;

    SuffixTree* st = new SuffixTree(text, n, p, x, rounds);
    cout << "Simple? " << st->simple() << endl;
    cout << "Number of nodes: " << st->get_num_nodes() << endl;
    //st->print(st->get_root(), 0);
    delete text;
    delete st;

    //auto stop = chrono::high_resolution_clock::now();
    //auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    //cout << "Time: " << duration.count() << " milliseconds" << endl;
    return 0;
}
