#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "automata.h"

DFA dfa;
NDFA ndfa;
set<string> tokens, final;

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
  src = states[""]; final.insert(src); //scr is a final state
  printf("final state: %s\n", src.c_str());
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
    final.insert(current.name);
    current++;
  }
  return 1;
}

void debugf() {
  printf("\n\nEstados finais:\n");
  for (set<string>::iterator i = final.begin(); i != final.end(); i++)
    printf("%s\n", i -> c_str());
}

void makedet() {
  int i, j, k;
  set<string> done;
  vector<string> states;
  states.push_back("S|"); done.insert("S|");
  for (i = 0; i < (int) states.size(); i++) {
    j = 0;
    string src;
    vector< set<string> > trans(312);
    while (j < (int) states[i].length()) {
      k = j;
      // searching for the first '|' in order to
      // find the 'src' state to look at its transitions
      // 'j' stops 1 position after that '|'
      while (states[i][j++] != '|');
      src = states[i].substr(k, j - k - 1);
      if (final.find(src) != final.end()) continue;
      for (ntransition::iterator t = ndfa[src].begin(); t != ndfa[src].end(); t++) {
        string tgt;
        for (k = 0; k < (int) (t -> second).size(); k++)
          tgt.append((t -> second)[k]).append("|");
        trans[(int) t -> first].insert(tgt);
      }
    }
    for (j = 0; j < 312; j++) {
      string tr;
      if (trans[j].size() == 0) continue;
      for (set<string>::iterator s = trans[j].begin(); s != trans[j].end(); s++)
        tr.append(*s);
      dfa[states[i]][(char) j] = tr;
      if (done.find(tr) == done.end()) {
        states.push_back(tr);
        done.insert(tr);
      }
    }
  }
  // for (auto fs: final) {
  // for (set<string>::iterator fs = final.begin(); fs != final.end(); fs++) {
  //   printf("+++++++++++++++= %s\n", fs -> c_str());
  //   for (auto s: dfa) {
  //     printf("%s\n", s.first.c_str());
  //   }
  // }
}

int minimize(string u) {
  int i; bool flag = false, tmp;
  for (i = 33; i < 127; i++, flag |= !tmp)
    if ((tmp = !minimize(dfa[u][(char) i]))) dfa[u][(char) i] = X;
  if (final.find(u) != final.end() || flag) return 1;
  dfa.erase(u);
  return 0;
}
