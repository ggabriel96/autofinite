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

void debugd() {
  for (DFA::iterator src = dfa.begin(); src != dfa.end(); src++) {
    printf("%s ", (src -> first).c_str());
    for (dtransition::iterator tgt = (src -> second).begin(); tgt != (src -> second).end(); tgt++)
        printf("(%c, %s)", tgt -> first, (tgt -> second).c_str());
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
      printf("%c%s%s", term, states[tgt].c_str(), s[i + 1] == '\0' ? "\n" : " | ");
    }
    printf("\n");
  }
  src = states[""];
  printf("readgrammar X: %s\n", src.c_str());
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
    printf("readtokens X: %s\n", current.name.c_str());
    for (i = 33; i < 127; i++)
      ndfa[current.name][(char) i].push_back(X);
    current++;
  }
  return 1;
}

void makedet() {
  int i, j, k;
  vector<string> states;
  states.push_back("S|");
  for (i = 0; i < (int) states.size(); i++) {
    j = 0;
    string src;
    set<string> z;
    vector<string> trans(312, "");
    while (j < (int) states[i].length()) {
      k = j;
      while (states[i][j++] != '|');
      src = states[i].substr(k, j - k - 1);
      //printf("src: %s\n", src.c_str());
      for (ntransition::iterator t = ndfa[src].begin(); t != ndfa[src].end(); t++) {
        string tgt;
        printf("\nt -> first %c\n", t -> first);
        for (k = 0; k < (int) (t -> second).size(); k++) {
          printf("%s, %d\n", (t -> second)[k].c_str(), z.find((t -> second)[k]) == z.end());
          if (z.insert((t -> second)[k]).second == true) {
            tgt.append((t -> second)[k]).append("|");
            printf("tgt %s\n", tgt.c_str());
          }
        }
        trans[(int) t -> first].append(tgt);
        printf("final tgt %s\n", trans[(int) t -> first].c_str());
        //printf("%s\n", tgt.c_str());
      }
    }
    for (j = 0; j < 312; j++) {
      if (trans[j].length() == 0) continue;
      dfa[states[i]][(char) j] = trans[j];
      states.push_back(trans[j]);
    }
  }
}
