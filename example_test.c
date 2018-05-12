/* <setjmp.h> and <stdio.h> header currently needed by ut */
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>

/* include unit testing framework */
#include "ut.h"

/* include unit to test */
#include "example.h"

/* test entry point is void test(void) */
void
test(void)
{
  UT_ASSERT(1 == somefunction_1(1)); // fails
  UT_ASSERT(2 == somefunction_1(1)); // succeeds

  // UT_ASSERT_SIGNAL(SIGABRT, somefunction_2()); // would fail completely
  UT_ASSERT_SIGNAL(SIGINT, somefunction_2()); // succeeds

  UT_ASSERT_EXIT(0, somefunction_3()); // fails
  UT_ASSERT_EXIT(1, somefunction_3()); // succeeds
}
