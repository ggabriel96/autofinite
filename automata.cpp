#include <stdio.h>
#include <string.h>
#include "automata.h"

NDFA ndfa;
vector<string> tokens;

struct enumstate{
  vector<char> name;
  string s;
  enumstate() { incremento();  }
  void operator++() { incremento(); }
  void incremento() {
    int i;
    do {
      for (i = 0; i < (int)name.size(); i++)
        if (name[i] < 'Z') { name[i]++; break; }
        else name[i] = 'A';
      if (i == (int)name.size()) name.push_back('A');
      s = string(name.begin(), name.end());
    } while(ndfa.find(s) != ndfa.end());
  }
};

void debugprint() {
  for (NDFA::iterator src = ndfa.begin(); src != ndfa.end(); src++) {
    printf("%s ", (src -> first).c_str());
    for (transition::iterator tgt = (src -> second).begin(); tgt != (src -> second).end(); tgt++)
      for (int i = 0; i < (int) (tgt -> second).size(); i++)
        printf("(%c, %s)", tgt -> first, (tgt -> second)[i].c_str());
    printf("\n");
  }}

int readgrammar() {
  int i, top;
  string src, tgt;
  char state[MAX], flag, word[MAX], term;
  while (fgets(state, MAX, stdin), strlen(state) > 1) {
    state[strlen(state) - 1] = '|';
    for (top = i = 0; state[i] != ':'; i++) {
      if (state[i] == '<' || state[i] == '>' || state[i] == ' ') continue;
      word[top++] = state[i];
    }
    word[top] = '\0';
    printf("%s ::= ", word);
    src = string(word);
    i += 2; //We have to change it, because there may be spaces between the characters ::=
    while (state[++i] != '\0') {
      for (top = flag = 0; state[i] != '|'; i++) {
        if (state[i] == ' ') continue;
        if (state[i] == '<') flag = 1;
        else if (state[i] == '>') flag = 0;
        else if (!flag) term = state[i];
        else word[top++] = state[i];
      }
      word[top] = '\0';
      tgt = string(word);
      ndfa[src][term].push_back(tgt);
      printf("%c%s |", term, word);
    }
    printf("\n");
  }
  return 1;
}

int readtokens() {
  char word[MAX];
  enumstate current;
  while (fgets(word, MAX, stdin) != NULL) {
    word[strlen(word) - 1] = '\0';
    tokens.push_back(string(word));
    printf("%s\n", word);
  }
  for (int i = 0; i < 30; ++current, i++) {
    for (int j = 0; j < (int)current.name.size(); j++)
      putchar(current.name[j]);
    putchar('\n');
  }
  return 1;
}
