//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03

// <future>

// class shared_future<R>

// void wait() const;

#include <cassert>
#include <chrono>
#include <future>
#include <ratio>

#include "make_test_thread.h"
#include "test_macros.h"

void func1(std::promise<int> p)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    p.set_value(3);
}

int j = 0;

void func3(std::promise<int&> p)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    j = 5;
    p.set_value(j);
}

void func5(std::promise<void> p)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    p.set_value();
}

int main(int, char**)
{
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::duration<double, std::milli> ms;
    ms Tolerance = ms(5);
#if defined(TEST_HAS_SANITIZERS) || TEST_SLOW_HOST()
    Tolerance *= 4;
#endif
    {
        typedef int T;
        std::promise<T> p;
        std::shared_future<T> f = p.get_future();
        support::make_test_thread(func1, std::move(p)).detach();
        assert(f.valid());
        f.wait();
        assert(f.valid());
        Clock::time_point t0 = Clock::now();
        f.wait();
        Clock::time_point t1 = Clock::now();
        assert(f.valid());
        assert(t1-t0 < Tolerance);
    }
    {
        typedef int& T;
        std::promise<T> p;
        std::shared_future<T> f = p.get_future();
        support::make_test_thread(func3, std::move(p)).detach();
        assert(f.valid());
        f.wait();
        assert(f.valid());
        Clock::time_point t0 = Clock::now();
        f.wait();
        Clock::time_point t1 = Clock::now();
        assert(f.valid());
        assert(t1-t0 < Tolerance);
    }
    {
        typedef void T;
        std::promise<T> p;
        std::shared_future<T> f = p.get_future();
        support::make_test_thread(func5, std::move(p)).detach();
        assert(f.valid());
        f.wait();
        assert(f.valid());
        Clock::time_point t0 = Clock::now();
        f.wait();
        Clock::time_point t1 = Clock::now();
        assert(f.valid());
        assert(t1-t0 < Tolerance);
    }

  return 0;
}
