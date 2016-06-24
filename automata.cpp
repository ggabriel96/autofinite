#include <set>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "automata.h"

FA fa;
map<string, set<char> > frst, fllw;

int readgrammar() {
  int i, top;
  string src, tgt;
  char s[MAX], word[MAX], flag;
  while (fgets(s, MAX, stdin) != NULL && strlen(s) > 1) {
    s[strlen(s) - 1] = '|';
    for (top = i = 0; s[i] != ':'; i++) {
      if (s[i] == '<' || s[i] == '>' || s[i] == ' ') continue;
      word[top++] = s[i];
    }
    word[top] = '\0';
    i += 2;
    src = string(word);
    printf("%s ::= ", src.c_str());
    vector < transition > prod;
    while (s[++i] != '\0') {
      transition n;
      for (top = flag = 0; s[i] != '|'; i++) {
        if (s[i] == ' ') continue;
        if (s[i] == '<') flag = 1;
        else if (s[i] == '>') {
          word[top] = '\0'; flag = 0;
          n.append(string(word));
          printf("%s", word);
          top = 0;
        } else if (!flag) {
          n.append(s[i]);
          printf("%c", s[i]);
        } else word[top++] = s[i];
      }
      printf(" | ");
      prod.push_back(n);
    }
    printf("\n");
    fa[src] = prod;
  }
  return 1;
}

void csv(void) {
  FILE *f = fopen("automata.csv", "w");
  fprintf(f, "State");
  fprintf(f, "\n");
  fclose(f);
}

void first(void) {
  int i, j;
  bool eps, done = false;
  while (!done) {
    done = true;
    for (auto& s: fa) {
      vector<transition> prod = s.second;
      for (i = 0; i < (int) prod.size(); i++) {
        // prod[i] é uma produção de s.first
        for (j = 0; j < (int) prod[i].sym.size(); j++) {
          eps = false;
          // aqui estamos iterando sobre todos
          // os símbolos de uma produção
          // 2 ou 6) se for terminal ou eps, adiciona ao first
          if (prod[i].sym[j].flag == TERMINAL) {
            done &= !frst[s.first].insert(prod[i].sym[j].t.front()).second;
            break;
          } else {
            // 3) se for não-terminal, olho pro first dele
            for (auto& sym: frst[prod[i].sym[j].t]) {
              // iterando sobre os chars do first
              // 5) se for eps, olhar para o próximo
              // símbolo de prod[i]
              if (sym == '&') { eps = true; continue; }
              // 4) se for terminal (não eps), adicionar ao first
              done &= !frst[s.first].insert(sym).second;
            }}
          // 7) se eu havia encontrado um eps e não
          // há próximo símbolo, adiciona eps ao first
          if (eps && j + 1 == (int) prod[i].sym.size())
            done &= !frst[s.first].insert('&').second;
          else if (!eps) break;
        }}}}}

void printfrst() {
  for (auto& s: frst) {
    printf("%s = ", s.first.c_str());
    for (auto& prod: s.second)
      printf("%c, ", prod);
    printf("\n");
  }}

void follow(void) {
  int i, j;
  bool done = false;
  // 1ª etapa
  fllw[S].insert('$');
  for (auto& s: fa) {
    vector<transition> prod = s.second;
    for (i = 0; i < (int) prod.size(); i++)
      // prod[i] é uma produção de s.first
      for (j = 0; j < (int) prod[i].sym.size() - 1; j++)
        // aqui estamos iterando sobre todos
        // os símbolos de uma produção
        if (prod[i].sym[j].flag == NTERMINAL) {
          if (prod[i].sym[j + 1].flag == TERMINAL)
            fllw[prod[i].sym[j].t].insert(prod[i].sym[j + 1].t.front());
          else if (prod[i].sym[j + 1].flag == NTERMINAL)
            for (auto& f: frst[prod[i].sym[j + 1].t])
              if (f != '&') fllw[prod[i].sym[j].t].insert(f);
        }}
  while (!done) {
    done = true;
    for (auto& s: fa) {
      vector<transition> prod = s.second;
      for (i = 0; i < (int) prod.size(); i++) {
        // prod[i] é uma produção de s.first
        int last = prod.size() - 1;
        if (prod[i].sym[last].flag == NTERMINAL) {
          for (auto& fr: fllw[s.first])
            if (fr != '&') done &= !fllw[prod[i].sym[last].t].insert(fr).second;
          if (frst[prod[i].sym[last].t].find('&') != frst[prod[i].sym[last].t].end()
              && prod[i].sym[last - 1].flag == NTERMINAL)
            for (auto& fr: fllw[s.first])
              if (fr != '&') done &= !fllw[prod[i].sym[last - 1].t].insert(fr).second;
        }}}}}

void printfllw() {
  for (auto& s: fllw) {
    printf("%s = ", s.first.c_str());
    for (auto& prod: s.second)
      printf("%c, ", prod);
    printf("\n");
  }}

void printfa() {
  int i, j;
  for (auto& s: fa) {
    printf("<%s> ::= ", s.first.c_str());
    vector<transition> prod = s.second;
    for (i = 0; i < (int) prod.size(); i++) {
      for (j = 0; j < (int) prod[i].sym.size(); j++)
        printf("%s", prod[i].sym[j].flag == TERMINAL ?
               prod[i].sym[j].t.c_str() :
               string("<").append(prod[i].sym[j].t).append(">").c_str());
      printf("%s", i + 1 < (int) prod.size() ? " | " : "\n");
    }}}
