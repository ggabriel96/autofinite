#include <set>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "automata.h"

FA fa;
map<string, set<char> > first, follow;

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
  bool eps;
  for (auto& s: fa) {
    vector<transition> prod = s.second;
    for (i = 0; i < (int) prod.size(); i++) {
      // prod[i] é uma produção de s.first
      // essa iteraçao com j não deve acontecer,
      // pois devemos olhar, inicialmente,
      // apenas para o primeiro símbolo da produção
      // e apenas aos próximos em algumas condições abaixo
      for (j = 0; j < (int) prod[i].sym.size(); j++) {
        eps = false;
        // aqui estamos iterando sobre todos
        // os símbolos de uma produção
        // 2 ou 6) se for terminal ou eps, adiciona ao first
        if (prod[i].sym[j].flag == TERMINAL)
          first[s.first].insert(prod[i].sym[j].t);
        else {
          // map<string, set<char> > first, follow;
          // 3) se for não-terminal, olho pro first dele
          for (auto& f: first[prod[i].sym[j].t]) {
            // iterando sobre os chars do first
            for (auto& sym: f.second) {
              // 5) se for eps, olhar para o próximo
              // símbolo de prod[i]
              if (!sym.t.compare("&")) { eps = true; break; }
              // 4) se for terminal (não eps), adicionar ao first
              first[s.first].insert(sym.t);
            }
            if (eps) break;
          }
        }
        // 7) se eu havia encontrado um eps e não
        // há próximo símbolo, adiciona eps ao first
        if (eps && j + 1 == (int) prod[i].sym.size())
          first[s.first] = "&";
      }
    }
  }
  return;
}

void follow(void) {
  return;
}

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
    }
  }
}
