#include <set>
#include <stdio.h>
#include <string.h>
#include "automata.h"

DFA dfa;
NDFA ndfa;
set<string> tokens;

struct enumstate {
  string name;
  enumstate() { name = string(); incremento(); }
  string& operator++() { incremento(); return name; }
  string operator++(int) { string tmp(name); incremento(); return tmp; }
  void incremento() {
    int i;
    do {
      for (i = 0; i < (int) name.length(); i++)
        if (name[i] < 'Z') { name[i]++; break; }
        else name[i] = 'A';
      if (i == (int) name.length()) name.append("A");
    }
    while (!name.compare(S) || !name.compare(X) ||
           ndfa.find(name) != ndfa.end());
  }
};

void debugprint() {
  for (NDFA::iterator src = ndfa.begin(); src != ndfa.end(); src++) {
    printf("%s ", (src -> first).c_str());
    for (ntransition::iterator tgt = (src -> second).begin(); tgt != (src -> second).end(); tgt++)
      for (int i = 0; i < (int) (tgt -> second).size(); i++)
        printf("(%c, %s)", tgt -> first, (tgt -> second)[i].c_str());
    printf("\n");
  }
}

int readgrammar() {
  int i, top;
  string src, tgt;
  enumstate state;
  map<string, string> states;
  char s[MAX], word[MAX], flag, term;
  states[S] = S; states[X] = X;
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
    //    printf("%s ::= ", states[src].c_str());
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
      //      printf("%c%s%s", term, states[tgt].c_str(), s[i + 1] == '\0' ? "\n" : " | ");
    }
    printf("\n");
  }
  src = states[""];
  for (i = 33; i < 127; i++)
    ndfa[src][(char) i].push_back(X);
  return 1;
}

int readtokens(int N) {
  int i;
  char word[MAX];
  enumstate current;
  while (N--) {
    if (fgets(word, MAX, stdin) == NULL || word[0] == '\n') return 0;
    word[strlen(word) - 1] = '\0';
    // map.insert().second returns true or false if
    // successfully inserted that key or it was
    // already there (and thus did nothing), respectively
    if (!tokens.insert(string(word)).second) continue;
    printf("%s\n", word);
    ndfa[S][word[0]].push_back(current.name);
    for (i = 1; word[i] != '\0'; i++)
      ndfa[current++][word[i]].push_back(current.name);
    for (i = 33; i < 127; i++)
      ndfa[current.name][(char) i].push_back(X);
  }
  return 1;
}

void makedet() {
  for (NDFA::iterator src = ndfa.begin(); src != ndfa.end(); src++) {
    printf("%s\n", (src -> first).c_str());
    for (ntransition::iterator tgt = (src -> second).begin(); tgt != (src -> second).end(); tgt++) {
      string dtgt;
      printf("%c: ", tgt -> first);
      for (int i = 0; i < (int) (tgt -> second).size(); i++) {
        dtgt.append("<").append((tgt -> second)[i].c_str()).append(">");
      }
      printf("%s\n", dtgt.c_str());
    }
    printf("\n");
  }
}
