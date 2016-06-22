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
  printf("\n----------------\nAutômato determinizado:\n");
  makedet();
  debugd();
  minimize();
  printf("\n----------------\nAutômato minimizado:\n");
  minimize();
  debugd();
  fill();
  csv();
  return 0;
}
