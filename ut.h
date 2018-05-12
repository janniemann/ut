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
extern volatile sig_atomic_t ut__signal_received;
extern jmp_buf ut__jmp_buf_env;
extern int ut__exit_status;

#define UT_ASSERT(expression) ut__tests_run++; ((expression) ? (void)0 : ut__assert(__FILE__, __LINE__, __func__, #expression))

#define UT_ASSERT_EXIT(x, e) do {					\
    if (!setjmp(ut__jmp_buf_env)) {					\
      ut__tests_run++;							\
      (e);								\
      fprintf(stderr, "UT: %s:%d:%s: expected exit status %d, but exit or _Exit was not called\n", __FILE__, __LINE__, __func__, x); \
      ut__tests_failed++;						\
    } else {								\
      if (ut__exit_status != x) {					\
	fprintf(stderr, "UT: %s:%d:%s: expected exit status %d, got exit status %d instead\n", __FILE__, __LINE__, __func__, x, ut__exit_status); \
	ut__tests_failed++;						\
      }									\
    }									\
  } while (0)


#define UT_ASSERT_SIGNAL(s, e) do {					\
    ut__tests_run++;							\
    ut__signal_received = -1;						\
    signal(s, ut__signal_handler);					\
    (e);								\
    if (ut__signal_received != s) {					\
      if (ut__signal_received == -1) {					\
	fprintf(stderr, "UT: %s:%d:%s: expected signal %d, but no signal was raised\n", __FILE__, __LINE__, __func__, s); \
      } else {								\
	fprintf(stderr, "UT: %s:%d:%s: expected signal %d, got signal %d instead\n", __FILE__, __LINE__, __func__, s, ut__signal_received); \
      }									\
      ut__tests_failed++;						\
    }									\
    signal(s, SIG_DFL);							\
  } while (0)


// forward declaration: entry point for tests:
extern void test(void);

// prototypes
void ut__signal_handler(int signal);
void ut__assert(const char * restrict file, int line, const char * restrict func, const char * restrict failed_expression);

#endif
