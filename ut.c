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


// --- simple assertions

// for recording tests and their failures
int ut__tests_run = 0;
int ut__tests_failed = 0;

// most basic assertion, based on <assert.h>
void
ut__assert(const char * restrict file, int line, const char * restrict func, const char * restrict failedexpr)
{
  (void)fprintf(stderr, "%s:%d:%s: test failure: %s\n", file, line, func, failedexpr);
  ut__tests_failed++;
}



// --- signal handling
volatile sig_atomic_t ut__signal;
void
ut__sighdlr(int sig)
{
  ut__signal = sig;
}

// --- exit status handling
jmp_buf ut__jmp_buf_env;

void
exit(int status)
{
  longjmp(ut__jmp_buf_env, (status & 0xff) + 0x100 + 1);
}

void
_Exit(int status)
{
  longjmp(ut__jmp_buf_env, (status & 0xff) + 0x100 + 1);
}





// --- entry point

// we provide our own "main" entry point to properly report test failures:
// the unit test is also unsuccessful if no tests where run
int
main(int argc, char **argv)
{
  fprintf(stderr, "UT: ut, version 1.0.0\n");

  // see forward declaration in ut.h
  test();

  fprintf(stderr, "UT: %d out of %d tested assertions failed.\n", ut__tests_failed, ut__tests_run);
  fprintf(stderr, "UT: %d out of %d tested assertions successful.\n", ut__tests_run - ut__tests_failed, ut__tests_run);

  if ((ut__tests_failed > 0) || (ut__tests_run == 0)) {
      return EXIT_FAILURE;
  } else {
      return EXIT_SUCCESS;
  }
}


