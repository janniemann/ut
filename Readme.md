# libut - simple library for unit testing ANSI C

[![Build Status](https://travis-ci.org/janniemann/ut.svg?branch=master)](https://travis-ci.org/janniemann/ut)

## Features & Goals

- simple
    - one header, one source file
    - less than 200 lines
    - no test fixtures
- easy to use
    - only three basic macros
    - entry point is void test(void);
    - uses names starting with ut_ or UT_, names starting with ut__ or UT__ are for internal use.
- fast
    - no recompilation of already compiled code

## Installation

- installation to ${HOME}/local:
    $ make install

- installation another location (use same PREFIX on both invocations of make):
    `$ make PREFIX=/usr/local`
    and as root:
    `# make PREFIX=/usr/local install`


## Usage

see GNUmakefile for target unit_test that demonstrates basic functionality.

simple Assertions:
- UT_ASSERT(expression)

assert exit status from a function call:
- UT_ASSERT_EXIT(status, call)

assert thrown signal:
- UT_ASSERT_SIGNAL(signal, call)

Note that calls to exit() or _Exit() in the unit to be tested will not perform any cleanup at all.
This may or may not break further tests. I don't know. Try it and let me know how it works for you.

In order to test for raised signals the signal handler will be overwritten by ut.
Setting your own signal handlers in the unit to be tested will not work.
Signal handlers will be reset to SIG_DFL after a successful test.
Testing for the wrong signal may abort the test run.
To remove some of those limitations it would be neccessary to:
- iterate over all implemented signals
- know how the signal is handled:
    - handling of some signal (SIGFPE) is undefined
    - some signal are uncatchable (SIGKILL)
- save the old signal handler

Bug reports and patches welcome.
