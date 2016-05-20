#include <set>
#include <stdio.h>
#include <string.h>
#include "automata.h"

NDFA ndfa;
set<string> tokens;

struct enumstate {
  string name;
  enumstate() { incremento(); }
  string& operator++() { incremento(); return name; }
  string operator++(int) { string tmp(name); incremento(); return tmp; }
  void incremento() {
    int i;
    do {
      for (i = 0; i < (int) name.length(); i++)
        if (name[i] < 'Z') { name[i]++; break; }
        else name[i] = 'A';
      if (i == (int) name.length()) name.append("A");
    } while(ndfa.find(name) != ndfa.end());
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
  int i, j, top;
  string src, tgt;
  enumstate state;
  map<string, string> states;
  char s[MAX], word[MAX], flag, term;
  while (fgets(s, MAX, stdin) != NULL && strlen(s) > 1) {
    s[strlen(s) - 1] = '|';
    for (top = i = 0; s[i] != ':'; i++) {
      if (s[i] == '<' || s[i] == '>' || s[i] == ' ') continue;
      word[top++] = s[i];
    }
    word[top] = '\0';
    //    for (j = i + 1; word[j] != '='; j++);
    //    if (j - i != 2) return 0;
    //    i = j + 1;
    i += 2;
    src = string(word);
    printf("%s ::= ", states[src].c_str());
    if (states.find(src) == states.end()) states[src] = state++;
    while (s[++i] != '\0') {
      for (top = flag = 0; s[i] != '|'; i++) {
        if (s[i] == ' ') continue;
        if (s[i] == '<') flag = 1;
        else if (s[i] == '>') flag = 0;
        else if (!flag) term = s[i];
        else word[top++] = s[i];
      }
      word[top] = '\0';
      tgt = string(word);
      if (states.find(tgt) == states.end()) states[tgt] = state++;
      ndfa[states[src]][term].push_back(states[tgt]);
      printf("%c%s |", term, states[tgt].c_str());
    }
    printf("\n");
  }
  return 1;
}

int readtokens(int N) {
  int i;
  char word[MAX];
  enumstate current;
  while (N--) {
    if (fgets(word, MAX, stdin) == NULL || word[0] == '\n') return 0;
    word[strlen(word) - 1] = '\0';
    if (!tokens.insert(string(word)).second) continue;
    printf("%s\n", word);
    ndfa[S][word[0]].push_back(current.name);
    for (i = 1; word[i] != '\0';)
      ndfa[current.name][word[i]].push_back(++current);
  }
  return 1;
}
