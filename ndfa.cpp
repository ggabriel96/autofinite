#include <stdio.h>
#include "automata.h"

int main(void) {
  int N;
  FA fa;
  FA dfa;
  while (scanf("%d\n", &N) != EOF) {
    // if (N && readtokens(N)) continue;
    if (readgrammar(fa)) continue;
    printf("Invalid input\n"); break;
  }
  printf("Original automaton:\n");
  printfa(fa);
  printf("\nAfter determinization:\n");
  dfa = makedet(fa);
  printfa(dfa);
  return 0;
}
