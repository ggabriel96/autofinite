#include <set>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "automata.h"

struct enumstate {
  FA fa;
  string name;
  enumstate(FA &fa) { this -> fa = fa; name = string(); inc(); }
  string& operator++() { inc(); return name; }
  string operator++(int) { string tmp = name; inc(); return tmp; }
  void inc() {
    int i;
    do {
      for (i = 0; i < (int) name.length(); i++)
        if (name[i] < 'Z') { name[i]++; break; }
        else name[i] = 'A';
      if (i == (int) name.length()) name.append("A");
    }
    while (!name.compare("S") || !name.compare("X") || fa.find(name) != fa.end());
  }
};

int readgrammar(FA &fa) {
  int i, top;
  string src, tgt;
  enumstate nextname(fa);
  map<string, string> newname;
  char s[MAX], word[MAX], flag;
  newname["S"] = "S";
  while (fgets(s, MAX, stdin) != NULL && strlen(s) > 1) {
    s[strlen(s) - 1] = '|';
    for (top = i = 0; s[i] != ':'; i++) {
      if (s[i] == '<' || s[i] == '>' || s[i] == ' ') continue;
      word[top++] = s[i];
    }
    word[top] = '\0';
    i += 2;
    src = string(word);
    if (newname.find(src) == newname.end()) newname[src] = nextname++;
    src = newname[src];
    // printf("%s ::= ", src.c_str());
    vector < transition > prod;
    while (s[++i] != '\0') {
      transition n;
      for (top = flag = 0; s[i] != '|'; i++) {
        if (s[i] == ' ') continue;
        if (s[i] == '<') flag = 1;
        else if (s[i] == '>') {
          word[top] = '\0'; flag = 0;
          string w = string(word);
          if (newname.find(w) == newname.end()) newname[w] = nextname++;
          w = newname[w];
          n.append(w);
          // printf("%s", w.c_str());
          top = 0;
        } else if (!flag) {
          n.append(s[i]);
          // printf("%c", s[i]);
        } else word[top++] = s[i];
      }
      // printf(" | ");
      prod.push_back(n);
    }
    // printf("\n");
    if (fa.find(src) == fa.end()) fa[src] = prod;
    else for (auto& p: prod) fa[src].push_back(p);
  }
  return 1;
}

// void csv(void) {
//   FILE *f = fopen("first.csv", "w");
//   for (auto& s: frst) {
//     fprintf(f, "%s = {", s.first.c_str());
//     for (set<char>::iterator c = frst[s.first].begin(); c != frst[s.first].end(); c++)
//       fprintf(f, "%c%s", *c, next(c) != frst[s.first].end() ? ", " : "}\n");
//   }
//   fclose(f);
//   f = fopen("follow.csv", "w");
//   for (auto& s: fllw) {
//     fprintf(f, "%s = {", s.first.c_str());
//     for (set<char>::iterator c = fllw[s.first].begin(); c != fllw[s.first].end(); c++)
//       fprintf(f, "%c%s", *c, next(c) != fllw[s.first].end() ? ", " : "}\n");
//   }
//   fclose(f);
// }

FA makedet(FA &fa) {
  FA dfa;
  int i, j, q;
  bool termonly = false;
  set<string> done;
  vector<string> states;
  map<string, map<string, set<string> > > dter;
  states.push_back("S"); done.insert("S");
  for (q = 0; q < (int) states.size(); q++) {
  // for (auto& s: fa) {
    // vector<transition> prod = s.second;
    vector<transition> prod = fa[states[q]];
    for (i = 0; i < (int) prod.size(); i++) {
      // prod[i] é uma produção de s.first
      if (prod[i].sym.size() == 1 && prod[i].sym[0].flag == TERMINAL) {
        termonly = true;
        // dter[s.first][prod[i].sym[0].t].insert("X");
        dter[states[q]][prod[i].sym[0].t].insert("X");
      }
      else {
        for (j = 0; j < (int) prod[i].sym.size() && prod[i].sym[j].flag == TERMINAL; j++);
        if (j > 0 && j < (int) prod[i].sym.size() && prod[i].sym[j].flag == NTERMINAL) {
          // dter[s.first][prod[i].sym[j - 1].t].insert(prod[i].sym[j].t);
          dter[states[q]][prod[i].sym[j - 1].t].insert(prod[i].sym[j].t);
          if (done.find(prod[i].sym[j].t) == done.end()) {
            states.push_back(prod[i].sym[j].t);
            done.insert(prod[i].sym[j].t);
          }
        }
      }
    }
  }
  for (auto& s: dter) {
    // printf("\n<%s>\n", s.first.c_str());
    vector < transition > prod;
    for (auto& t: s.second) {
      // printf("%s: ", t.first.c_str());
      transition n;
      n.append(t.first.front());
      string tr = "|";
      for (auto& nt: t.second) tr.append(nt).append("|");
      n.append(tr);
      // printf("%s\n", tr.c_str());
      prod.push_back(n);
    }
    dfa[string("|").append(s.first).append("|")] = prod;
  }
  if (termonly) dfa[string("|").append("X").append("|")] = vector<transition>();
  return dfa;
}

void printfa(FA &a) {
  int i, j;
  for (auto& s: a) {
    vector<transition> prod = s.second;
    printf("<%s>%s", s.first.c_str(), prod.size() == 0 ? "\n" : " ::= ");
    for (i = 0; i < (int) prod.size(); i++) {
      for (j = 0; j < (int) prod[i].sym.size(); j++)
        printf("%s", prod[i].sym[j].flag == TERMINAL ?
               prod[i].sym[j].t.c_str() :
               string("<").append(prod[i].sym[j].t).append(">").c_str());
      printf("%s", i + 1 < (int) prod.size() ? " | " : "\n");
    }}}
