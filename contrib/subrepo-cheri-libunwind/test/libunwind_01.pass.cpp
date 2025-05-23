// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// TODO: Investigate this failure on x86_64 macOS back deployment
// XFAIL: stdlib=apple-libc++ && target=x86_64-apple-macosx{{10.9|10.10|10.11|10.12|10.13|10.14|10.15|11.0|12.0}}

// TODO: Figure out why this fails with Memory Sanitizer.
// XFAIL: msan

#include <libunwind.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void backtrace(int lower_bound) {
  unw_context_t context;
  unw_getcontext(&context);

  unw_cursor_t cursor;
  unw_init_local(&cursor, &context);

  char buffer[1024];
  unw_word_t offset = 0;

  int n = 0;
  do {
    n++;
    if (unw_get_proc_name(&cursor, buffer, sizeof(buffer), &offset) == 0) {
      fprintf(stderr, "Frame %d: %s+%p\n", n, buffer, (void*)offset);
    } else {
      fprintf(stderr, "Frame %d: Could not get name for cursor\n", n);
    }
    if (n > 100) {
      fprintf(stderr, "ERROR: Got %d frames, but expected at most 100\n", n);
      abort();
    }
  } while (unw_step(&cursor) > 0);

  if (n < lower_bound) {
    fprintf(stderr, "ERROR: Got %d frames, but expected at least %d\n", n, lower_bound);
    abort();
  }
}

__attribute__((noinline)) void test1(int i) {
  fprintf(stderr, "starting %s\n", __func__);
  backtrace(i);
  fprintf(stderr, "finished %s\n", __func__); // ensure return address is saved
}

__attribute__((noinline)) void test2(int i, int j) {
  fprintf(stderr, "starting %s\n", __func__);
  backtrace(i);
  test1(j);
  fprintf(stderr, "finished %s\n", __func__); // ensure return address is saved
}

__attribute__((noinline)) void test3(int i, int j, int k) {
  fprintf(stderr, "starting %s\n", __func__);
  backtrace(i);
  test2(j, k);
  fprintf(stderr, "finished %s\n", __func__); // ensure return address is saved
}

void test_no_info() {
  unw_context_t context;
  unw_getcontext(&context);

  unw_cursor_t cursor;
  unw_init_local(&cursor, &context);

  unw_proc_info_t info;
  int ret = unw_get_proc_info(&cursor, &info);
  if (ret != UNW_ESUCCESS)
    abort();

  // Set the IP to an address clearly outside any function.
  unw_set_reg(&cursor, UNW_REG_IP, (unw_word_t)0);

  ret = unw_get_proc_info(&cursor, &info);
  if (ret != UNW_ENOINFO)
    abort();
}

void test_reg_names() {
  unw_context_t context;
  unw_getcontext(&context);

  unw_cursor_t cursor;
  unw_init_local(&cursor, &context);

  int max_reg_num = -100;
#if defined(__i386__)
  max_reg_num = 7;
#elif defined(__x86_64__)
  max_reg_num = 32;
#endif

  const char prefix[] = "unknown";
  for (int i = -2; i < max_reg_num; ++i) {
    if (strncmp(prefix, unw_regname(&cursor, i), sizeof(prefix) - 1) == 0)
      abort();
  }

  if (strncmp(prefix, unw_regname(&cursor, max_reg_num + 1),
              sizeof(prefix) - 1) != 0)
    abort();
}

#if defined(__x86_64__)
void test_reg_get_set() {
  unw_context_t context;
  unw_getcontext(&context);

  unw_cursor_t cursor;
  unw_init_local(&cursor, &context);

  for (int i = 0; i < 17; ++i) {
    const unw_word_t set_value = 7;
    if (unw_set_reg(&cursor, i, set_value) != UNW_ESUCCESS)
      abort();

    unw_word_t get_value = 0;
    if (unw_get_reg(&cursor, i, &get_value) != UNW_ESUCCESS)
      abort();

    if (set_value != get_value)
      abort();
  }
}

void test_fpreg_get_set() {
  unw_context_t context;
  unw_getcontext(&context);

  unw_cursor_t cursor;
  unw_init_local(&cursor, &context);

  // get/set is not implemented for x86_64 fpregs.
  for (int i = 17; i < 33; ++i) {
    const unw_fpreg_t set_value = 7;
    if (unw_set_fpreg(&cursor, i, set_value) != UNW_EBADREG)
      abort();

    unw_fpreg_t get_value = 0;
    if (unw_get_fpreg(&cursor, i, &get_value) != UNW_EBADREG)
      abort();
  }
}
#else
void test_reg_get_set() {}
void test_fpreg_get_set() {}
#endif

int main(int, char**) {
  test1(3);
  test2(3, 4);
  test3(3, 4, 5);
  test_no_info();
  test_reg_names();
  test_reg_get_set();
  test_fpreg_get_set();
  fprintf(stderr, "Success!\n");
  return 0;
}
