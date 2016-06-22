#include <stdio.h>
#include "automata.h"

int main(void) {
  readgrammar();
  printf("\n------------------\n");
  printfa();
  first(); follow(); csv();
  return 0;
}
