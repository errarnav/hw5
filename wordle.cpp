#ifndef RECCHECK
#include <iostream>
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// helper prototype
void helper(const string& in,
            string& current,
            string floating,
            int pos,
            const set<string>& dict,
            set<string>& results);

std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    string cur = in;
    helper(in, cur, floating, 0, dict, results);
    return results;
}

void helper(const string& in,
            string& current,
            string floating,
            int pos,
            const set<string>& dict,
            set<string>& results)
{
    int n = in.size();

    // Base case: full string formed
    if(pos == n) {

        // Must use ALL floating letters
        if(floating.empty() && dict.find(current) != dict.end()) {
            results.insert(current);
        }
        return;
    }

    // If fixed letter, skip
    if(in[pos] != '-') {
        helper(in, current, floating, pos + 1, dict, results);
        return;
    }

    // Count blank slots left
    int blanksLeft = 0;
    for(int i = pos; i < n; i++) {
        if(current[i] == '-') blanksLeft++;
    }

    // Prune impossible branches: not enough space for floating letters
    if((int)floating.size() > blanksLeft)
        return;

    // OPTION 1: place each floating letter here
    for(int i = 0; i < (int)floating.size(); i++) {
        char c = floating[i];

        current[pos] = c;
        string nextFloating = floating;
        nextFloating.erase(i, 1);  // remove used floating letter

        helper(in, current, nextFloating, pos + 1, dict, results);
    }

    // OPTION 2: place a non-floating letter *only if*
    // we still have more blanks than floating letters.
    if((int)floating.size() < blanksLeft) {
        for(char c = 'a'; c <= 'z'; c++) {
            // Don’t place a floating letter here in this branch
            if(floating.find(c) != string::npos)
                continue;

            current[pos] = c;
            helper(in, current, floating, pos + 1, dict, results);
        }
    }

    // restore for safety (not strictly required)
    current[pos] = '-';
}