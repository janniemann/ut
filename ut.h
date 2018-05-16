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

/* simple assert-like macro */
#define UT_ASSERT(expression) ut__tests_run++; ((expression) ? (void)0 : ut__assert(__FILE__, __LINE__, __func__, #expression))

/* assert exit status, note: due to stack protection a call to setjmp can not be hidden */
#define UT_ASSERT_EXIT(x, e) do {					\
    ut__tests_run++;							\
    ut__longjmp_from_exit = setjmp(ut__jmp_buf_env);			\
    if (!ut__longjmp_from_exit) {					\
      (e);								\
    }									\
    ut__check_status(__FILE__, __LINE__, __func__, x, #e);		\
  } while (0)

/* assert that a signal is raised */
#define UT_ASSERT_SIGNAL(s, e) do {					\
    ut__register_signal_test(s);					\
    (e);								\
    ut__check_signal(__FILE__, __LINE__, __func__, s, #e);		\
  } while (0)

/* prototype for test entry point */
extern void test(void);

/* the following prototypes and extern declarations are used by macros above */
extern int ut__tests_run;
extern jmp_buf ut__jmp_buf_env;
extern int ut__exit_status;
extern int ut__longjmp_from_exit;

void ut__assert(const char * restrict file, int line, const char * restrict func, const char * restrict failed_expression);
void ut__check_signal(const char * restrict file, int line, const char * restrict func, int expected_signal, const char * restrict stmt);
void ut__check_status(const char * restrict file, int line, const char * restrict func, int expected_status, const char * restrict stmt);
void ut__register_signal_test(int signal);

#endif
