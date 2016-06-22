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
<<<<<<< HEAD
  minimize("S|");
  remove();
  printf("\n----------------\nAutômato minimizado:\n");
=======
  debugf();
  minimize();
>>>>>>> f57bafd0c029848c5caac6129a1f0717941b5f5d
  debugd();
  fill();
  csv();
  return 0;
}
