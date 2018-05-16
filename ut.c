/*
 * Copyright (c) 2018 Jan Niemann <jan.niemann@beet5.de>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "ut.h"

/* --- recording tests and failures */
int ut__tests_run = 0;
int ut__tests_failed = 0;


/* --- simple assertion */

/* see UT_ASSERT */
void
ut__assert(const char * restrict file, int line, const char * restrict func, const char * restrict failed_expression)
{
  (void)fprintf(stderr, "UT: %s:%d:%s: failed expression: %s\n", file, line, func, failed_expression);
  ut__tests_failed++;
}

/* --- signal handling */

/* ut__signal_handler() records the received signal in ut_signal_received */
volatile sig_atomic_t ut__signal_received;
void ut__signal_handler(int signal);

void
ut__signal_handler(int signal)
{
  ut__signal_received = signal;
}

/* register a signal test */
void
ut__register_signal_test(int expected_signal)
{
  ut__tests_run++;
  ut__signal_received = -1;
  signal(expected_signal, ut__signal_handler);
}

/* check received signal */
void
ut__check_signal(const char * restrict file, int line, const char * restrict func, int expected_signal, const char * restrict stmt)
{
  if (ut__signal_received != expected_signal) {
    if (ut__signal_received == -1) {
      fprintf(stderr, "UT: %s:%d:%s: expected signal %d, but no signal was raised: %s\n", file, line, func, expected_signal, stmt);
    } else {
      fprintf(stderr, "UT: %s:%d:%s: expected signal %d, got signal %d instead: %s\n", file, line, func, expected_signal, ut__signal_received, stmt);
    }
    ut__tests_failed++;
  }
  signal(expected_signal, SIG_DFL);
}




/* --- exit status handling */

/* exit and _Exit now jump back to a location recorded by UT_ASSERT_EXIT */
jmp_buf ut__jmp_buf_env;
int ut__longjmp_from_exit;
int ut__exit_status;

void
exit(int status)
{
  ut__exit_status = status;
  longjmp(ut__jmp_buf_env, 1);
}

void
_Exit(int status)
{
  ut__exit_status = status;
  longjmp(ut__jmp_buf_env, 1);
}

/* check exit status afterwards */
void
ut__check_status(const char * restrict file, int line, const char * restrict func, int expected_status, const char * restrict stmt)
{
  if (0 == ut__longjmp_from_exit) {
    fprintf(stderr, "UT: %s:%d:%s: expected exit status %d, but exit or _Exit was not called: %s\n", file, line, func, expected_status, stmt);
    ut__tests_failed++;
  } else {
    if (ut__exit_status != expected_status) {
      fprintf(stderr, "UT: %s:%d:%s: expected exit status %d, got exit status %d instead: %s\n", file, line, func, expected_status, ut__exit_status, stmt); \
      ut__tests_failed++;
    }
  }
}


/* --- entry point */
int
main(int argc, char **argv)
{
  fprintf(stderr, "UT: ut, version 1.1.0\n");

  test();

  fprintf(stderr, "UT: %d out of %d tested assertions failed.\n", ut__tests_failed, ut__tests_run);
  fprintf(stderr, "UT: %d out of %d tested assertions successful.\n", ut__tests_run - ut__tests_failed, ut__tests_run);

  if ((ut__tests_failed > 0) || (ut__tests_run == 0)) {
      return EXIT_FAILURE;
  } else {
      return EXIT_SUCCESS;
  }
}
