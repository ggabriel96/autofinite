#include <stdio.h>
#include "automata.h"

int main(void) {
  int N;
  while (scanf("%d\n", &N) != EOF) {
    if (N && readtokens(N)) continue;
    if (readgrammar()) continue;
    printf("Invalid input\n"); break;
  }
  debugprint();
  return 0;
}
