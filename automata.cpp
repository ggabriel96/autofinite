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
  bool first;
  for (auto& src: ndfa) {
    first = true;
    printf("<%s> ::= ", src.first.c_str());
    for (auto& tgt: src.second) {
      for (int i = 0; i < (int) tgt.second.size(); i++) {
        if (!first) printf(" | "); first = false;
        printf("%c<%s>", tgt.first, tgt.second[i].c_str());
      }
    }
    printf("\n");
  }
}

void debugd() {
  bool first;
  for (auto& src: dfa) {
    first = true;
    printf("<%s> ::= ", src.first.c_str());
    for (auto& tgt: src.second) {
      if (!first) printf(" | "); first = false;
      printf("%c<%s>", tgt.first, tgt.second.c_str());
    }
    printf("\n");
  }
}

int readgrammar() {
  int i, top;
  string src, tgt;
  enumstate state;
  map<string, string> states;
  char s[MAX], word[MAX], flag, term = 0;
  states[S] = S; states[X] = X;
  while (fgets(s, MAX, stdin) != NULL && strlen(s) > 1) {
    s[strlen(s) - 1] = '|';
    for (top = i = 0; s[i] != ':'; i++) {
      if (s[i] == '<' || s[i] == '>' || s[i] == ' ') continue;
      word[top++] = s[i];
    }
    word[top] = '\0';
    i += 2;
    src = string(word);
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
    }
  }
  src = states[""]; final.insert(src); //scr is a final state
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
    ndfa[S][word[0]].push_back(current.name);
    for (i = 1; word[i] != '\0'; i++)
      ndfa[current++][word[i]].push_back(current.name);
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
      for (auto& s: trans[j])
        tr.append(s);
      dfa[states[i]][(char) j] = tr;
      if (done.find(tr) == done.end()) {
        states.push_back(tr);
        done.insert(tr);
      }
    }
  }
  done.clear();
  for (auto& fs: final) {
    for (auto& s: dfa)
      if (s.first.find(fs) != string::npos)
        done.insert(s.first);
    final.erase(fs);
  }
  for (auto& s: done) final.insert(s);
}

set<string> ok;
int minimize(string u) {
  bool flag = false;
  ok.insert(u);
  for (auto& v: dfa[u])
    if (ok.find(v.second) == ok.end())
      flag |= minimize(v.second);
    else flag |= (dfa.find(v.second) != dfa.end() && v.second.compare(u));
  if (final.find(u) != final.end()) flag = true;
  if (flag == false) dfa.erase(u);
  return flag;
}

void remove() {
  for (auto& u: dfa)
    for (auto& v: u.second)
      if (dfa.find(v.second) == dfa.end())
        u.second.erase(v.first);
}

void minimize() {
  minimize("S|");
  remove();
}

void fill() {
  char c;
  dtransition x;
  for (c = 33; c < 127; c++) x[c] = "X|";
  dfa["X|"] = x;
  for (auto& s: dfa)
    for (c = 33; c < 127; c++)
      if (s.second.find(c) == s.second.end())
        s.second[c] = "X|";
}

void csv(void) {
  char c;
  FILE *f = fopen("automata.csv", "w");
  fprintf(f, "State");
  for (c = 33; c < 127; c++) {
    if (c == '"') fprintf(f, " \"\"\"\"");
    else if (c == ',') fprintf(f, " \",\"");
    else fprintf(f, " %c", c);
  }
  for (auto& s: dfa) {
    fprintf(f, "\n%s", s.first.c_str());
    for (auto& t: s.second)
      fprintf(f, " %s", t.second.c_str());
  }
  fprintf(f, "\n");
  fclose(f);
}
