#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(
    const string& in,
    const string& floating,
    const set<string>& dict,
    string& current,
    int pos,
    set<string>& results);

bool containsAllFloating(const string& word, const string& floating);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    string current = in;
    wordleHelper(in, floating, dict, current, 0, results);
    return results;
}

// Define any helper functions here
void wordleHelper(
    const string& in,
    const string& floating,
    const set<string>& dict,
    string& current,
    int pos,
    set<string>& results)
{
    // Base case: if we've filled all positions
    if(pos == (int)in.length()) {
        // Check if current word contains all floating characters
        if(containsAllFloating(current, floating)) {
            // Check if it's in the dictionary
            if(dict.find(current) != dict.end()) {
                results.insert(current);
            }
        }
        return;
    }
    
    // If current position is fixed (not a dash), move to next position
    if(in[pos] != '-') {
        wordleHelper(in, floating, dict, current, pos + 1, results);
    }
    else {
        // Try all 26 lowercase letters at this position
        for(char c = 'a'; c <= 'z'; c++) {
            current[pos] = c;
            wordleHelper(in, floating, dict, current, pos + 1, results);
        }
        // Restore dash (backtrack)
        current[pos] = '-';
    }
}

bool containsAllFloating(const string& word, const string& floating) {
    for(char c : floating) {
        bool found = false;
        for(char w : word) {
            if(w == c) {
                found = true;
                break;
            }
        }
        if(!found) {
            return false;
        }
    }
    return true;
}