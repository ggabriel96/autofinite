#include <stdio.h>
#include "automata.h"

int main(void) {
  readgrammar();
  printf("Automaton:\n");
  printfa();
  printf("\n------------------\nFirst set:\n");
  first();
  printfrst();
  printf("\n------------------\nFollow set:\n");
  follow();
  printfllw();
  csv();
  return 0;
}
