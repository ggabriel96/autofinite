#include <map>
#include <string>
#include <vector>
using namespace std;

typedef map<char, string> dtransition;
typedef map<string, dtransition> DFA;
typedef map<char, vector<string> > ntransition;
typedef map<string, ntransition> NDFA;

#define MAX 112345
#define S "S"
#define X "X"

void debugprint();
void debugd();
void debugf();

// Read the grammar and build the NDFA
int readgrammar();

// Read the tokens and build the NDFA
int readtokens(int);

// Make the automaton nondeterministic
void makedet();

int minimize(string);
