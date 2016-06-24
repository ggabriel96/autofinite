#include <stdio.h>
#include "automata.h"

int main(void) {
  readgrammar();
  printf("\n------------------\n");
  printfa();
  printf("\n------------------\n");
  first();
  printfrst();
  printf("\n------------------\n");
  follow();
  printfllw();
  printf("\n------------------\n");
  csv();
  return 0;
}
