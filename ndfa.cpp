#include <stdio.h>
#include "automata.h"

int main(void) {
  int N;
  while (scanf("%d\n", &N) != EOF) {
    if (N && readtokens(N)) continue;
    if (readgrammar()) continue;
    printf("Invalid input\n"); break;
  }
  printf("IIIIIIII222222\n");
  debugprint();
  makedet();
  printf("IIIIIIIIIIII\n");
  // debugprint();
  printf("\n----------------\n");
  debugd();
  debugf();
  minimize(S);
  return 0;
}
