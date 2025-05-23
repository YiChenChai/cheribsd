//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, libcpp-no-rtti

// XFAIL: availability-bad_any_cast-missing && !no-exceptions

// <any>

// template <class T, class ...Args> any make_any(Args&&...);
// template <class T, class U, class ...Args>
// any make_any(initializer_list<U>, Args&&...);

#include <any>
#include <cassert>

#include "any_helpers.h"
#include "count_new.h"
#include "test_macros.h"

template <class Type>
void test_make_any_type() {
    // constructing from a small type should perform no allocations.
    DisableAllocationGuard g(isSmallType<Type>()); ((void)g);
    assert(Type::count == 0);
    Type::reset();
    {
        std::any a = std::make_any<Type>();

        assert(Type::count == 1);
        assert(Type::copied == 0);
        assert(Type::moved == 0);
        assertContains<Type>(a, 0);
    }
    assert(Type::count == 0);
    Type::reset();
    {
        std::any a = std::make_any<Type>(101);

        assert(Type::count == 1);
        assert(Type::copied == 0);
        assert(Type::moved == 0);
        assertContains<Type>(a, 101);
    }
    assert(Type::count == 0);
    Type::reset();
    {
        std::any a = std::make_any<Type>(-1, 42, -1);

        assert(Type::count == 1);
        assert(Type::copied == 0);
        assert(Type::moved == 0);
        assertContains<Type>(a, 42);
    }
    assert(Type::count == 0);
    Type::reset();
}

template <class Type>
void test_make_any_type_tracked() {
    // constructing from a small type should perform no allocations.
    DisableAllocationGuard g(isSmallType<Type>()); ((void)g);
    {
        std::any a = std::make_any<Type>();
        assertArgsMatch<Type>(a);
    }
    {
        std::any a = std::make_any<Type>(-1, 42, -1);
        assertArgsMatch<Type, int, int, int>(a);
    }
    // initializer_list constructor tests
    {
        std::any a = std::make_any<Type>({-1, 42, -1});
        assertArgsMatch<Type, std::initializer_list<int>>(a);
    }
    {
        int x = 42;
        std::any a  = std::make_any<Type>({-1, 42, -1}, x);
        assertArgsMatch<Type, std::initializer_list<int>, int&>(a);
    }
}

#ifndef TEST_HAS_NO_EXCEPTIONS

struct SmallThrows {
  SmallThrows(int) { throw 42; }
  SmallThrows(std::initializer_list<int>, int) { throw 42; }
};
static_assert(IsSmallObject<SmallThrows>::value, "");

struct LargeThrows {
  LargeThrows(int) { throw 42; }
  LargeThrows(std::initializer_list<int>, int) { throw 42; }
  int data[sizeof(std::any)];
};
static_assert(!IsSmallObject<LargeThrows>::value, "");

template <class Type>
void test_make_any_throws()
{
    {
        try {
            TEST_IGNORE_NODISCARD std::make_any<Type>(101);
            assert(false);
        } catch (int const&) {
        }
    }
    {
        try {
            TEST_IGNORE_NODISCARD std::make_any<Type>({1, 2, 3}, 101);
            assert(false);
        } catch (int const&) {
        }
    }
}

#endif

int main(int, char**) {
    test_make_any_type<small>();
    test_make_any_type<large>();
    test_make_any_type<small_throws_on_copy>();
    test_make_any_type<large_throws_on_copy>();
    test_make_any_type<throws_on_move>();
    test_make_any_type_tracked<small_tracked_t>();
    test_make_any_type_tracked<large_tracked_t>();
#ifndef TEST_HAS_NO_EXCEPTIONS
    test_make_any_throws<SmallThrows>();
    test_make_any_throws<LargeThrows>();

#endif

  return 0;
}
