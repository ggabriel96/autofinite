#include <map>
#include <string>
#include <vector>
using namespace std;

typedef map<char, vector<string> > transition;
typedef map<string, transition> NDFA;

#define MAX 112345
#define S "S"

void debugprint();
//Read the grammar and build the NDFA
int readgrammar();
//Read the tokens and build the NDFA
int readtokens(int);
