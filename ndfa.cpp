#include <stdio.h>
#include "automata.h"

int main(void) {
  int N;
  while (scanf("%d\n", &N) != EOF) {
    if (N && readtokens(N)) continue;
    if (readgrammar()) continue;
    printf("Invalid input\n"); break;
  }
  printf("Autômato original:\n");
  debugprint();
  makedet();
  printf("\n----------------\nAutômato determinizado:\n");
  debugd();
  minimize("S|");
  remove();
  printf("\n----------------\nAutômato minimizado:\n");
  debugd();
  fill();
  csv();
  return 0;
}
