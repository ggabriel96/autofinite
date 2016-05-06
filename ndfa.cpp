#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

#define MAX 112345
#define MAXS 1123
#define SIZE 20

typedef map<char, vector<string> > transition;
typedef map<string, transition> NDFA;

NDFA ndfa;
vector<string> tokens;

int main(void) {
  int T, i, top;
  string src, tgt;
  char state[MAX], flag, word[MAX], term;
  scanf("%d\n", &T);
  while (T--) {
    scanf("%s\n", word);
    tokens.push_back(string(word));
  }
  while (fgets(state, MAX, stdin) != NULL) {
    state[strlen(state) - 1] = '|';
    for (top = i = 0; state[i] != ':'; i++) {
      if (state[i] == '<' || state[i] == '>' || state[i] == ' ') continue;
      word[top++] = state[i];
    }
    word[top] = '\0';
    printf("%s ::= ", word);
    src = string(word);
    i += 2;
    while (state[++i] != '\0') {
      for (top = flag = 0; state[i] != '|'; i++) {
	if (state[i] == ' ') continue;
	if (state[i] == '<') flag = 1;
	else if (state[i] == '>') flag = 0;
	else if (!flag) term = state[i];
	else word[top++] = state[i];
      }
      word[top] = '\0';
      tgt = string(word);
      ndfa[src][term].push_back(tgt);
      printf("%c%s |", term, word);
    }
    printf("\n");
  }
  for (NDFA::iterator src = ndfa.begin(); src != ndfa.end(); src++) {
    printf("%s ", (src -> first).c_str());
    for (transition::iterator tgt = (src -> second).begin(); tgt != (src -> second).end(); tgt++)
      for (i = 0; i < (int) (tgt -> second).size(); i++)
	printf("(%c, %s)", tgt -> first, (tgt -> second)[i].c_str());
    printf("\n");
  }
  return 0;
}
