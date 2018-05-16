/* include unit testing framework */
#include <setjmp.h>
#include "ut.h"

/* additional header for this test */
#include <signal.h> /* SIGINT */

/* unit to test */
#include "example.h"

/* test entry point */
void
test(void)
{
  UT_ASSERT(1 == somefunction_1(1)); /* fails */
  UT_ASSERT(2 == somefunction_1(1)); /* succeeds */

  /* expecting the wrong signal is unsupported */
  /* UT_ASSERT_SIGNAL(SIGUSR1, somefunction_2()); */
  UT_ASSERT_SIGNAL(SIGINT, somefunction_2()); /* succeeds */
  UT_ASSERT_SIGNAL(SIGINT, (void)somefunction_1(1)); /* fails, expected signal not raised */

  UT_ASSERT_EXIT(1, somefunction_3()); /* succeeds */
  UT_ASSERT_EXIT(0, somefunction_3()); /* fails */
  UT_ASSERT_EXIT(1, (void)somefunction_1(1)); /* fails, not exiting */
}
