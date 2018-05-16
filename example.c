/* this file does not contain specific code for ut. */

#include <stdlib.h> /* for exit(3) and EXIT_FAILURE */
#include <signal.h> /* for raise and SIGINT */

#include "example.h" /* prototypes for below functions */

/* returns a + 1 */
int
somefunction_1(int a)
{
  return a+1;
}

/* raises SIGINT */
void
somefunction_2(void)
{
  raise (SIGINT);
}

/* calls exit with EXIT_FAILURE */
void
somefunction_3(void)
{
  exit(EXIT_FAILURE);
}
