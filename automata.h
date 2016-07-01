#include <map>
#include <string>
#include <vector>
using namespace std;

#define MAX 112345
#define TERMINAL 1
#define NTERMINAL 2

struct symbol {
  string t; int flag;
  symbol() {}
  symbol(char c) {
    t = string(1, c);
    flag = TERMINAL;
  }
  symbol(string s) {
    t = s; flag = NTERMINAL;
  }
};

struct transition {
  vector<symbol> sym;
  void append(symbol s) {
    sym.push_back(s);
  }
  void append(char c) {
    sym.push_back(symbol(c));
  }
  void append(string s) {
    sym.push_back(symbol(s));
  }
};

typedef map<string, vector<transition> > FA;

// Read the grammar and build the NDFA
int readgrammar(FA &);
FA makedet(FA &);
void printfa(FA &);
void csv();
