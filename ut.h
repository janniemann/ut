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

#ifndef UT__H
#define UT__H

extern int ut__tests_run;
extern int ut__tests_failed;
extern volatile sig_atomic_t ut__signal;
extern jmp_buf ut__jmp_buf_env;

#define UT_ASSERT(e) ut__tests_run++; ((e) ? (void)0 : ut__assert(__FILE__, __LINE__, __func__, #e))

#define UT_ASSERT_EXIT(x, e) do {					\
    int status = setjmp(ut__jmp_buf_env);				\
    switch (status) {							\
    case 0:								\
      ut__tests_run++;							\
      (e);								\
      break;								\
    case (0x100 + ((x) & 0xff)) + 1:					\
      break;								\
    default:								\
      status = ((status - 1) - 0x100) & 0xff;				\
      fprintf(stderr, "%s:%d:%s: unit test failure: expected exit status %d, got status %d\n", __FILE__, __LINE__, __func__, x, status); \
      ut__tests_failed++;						\
    }									\
  } while (0)


#define UT_ASSERT_SIGNAL(s, e) do {		\
    ut__tests_run++;				\
    signal(s, ut__sighdlr);			\
    (e);					\
    if (ut__signal != s) {			\
      ut__tests_failed++;			\
    }						\
    signal(s, SIG_DFL);				\
  } while (0)


// forward declaration: entry point for tests:
extern void test(void);

void ut__sighdlr(int sig);
void ut__assert(const char * restrict file, int line, const char * restrict func, const char * restrict failedexpr);

void exit(int status);
void _Exit(int status);

#endif
