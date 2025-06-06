//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03
// ALLOW_RETRIES: 2

// <mutex>

// template <class Mutex> class unique_lock;

// unique_lock(mutex_type& m, try_to_lock_t);

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <mutex>
#include <thread>

#include "make_test_thread.h"
#include "test_macros.h"

std::mutex m;

typedef std::chrono::system_clock Clock;
typedef Clock::time_point time_point;
typedef Clock::duration duration;
typedef std::chrono::milliseconds ms;
typedef std::chrono::nanoseconds ns;

void f()
{
    time_point t0 = Clock::now();
    {
        std::unique_lock<std::mutex> lk(m, std::try_to_lock);
        assert(lk.owns_lock() == false);
    }
    {
        std::unique_lock<std::mutex> lk(m, std::try_to_lock);
        assert(lk.owns_lock() == false);
    }
    {
        std::unique_lock<std::mutex> lk(m, std::try_to_lock);
        assert(lk.owns_lock() == false);
    }
    while (true)
    {
        std::unique_lock<std::mutex> lk(m, std::try_to_lock);
        if (lk.owns_lock())
            break;
    }
    time_point t1 = Clock::now();
    ns d = t1 - t0 - ms(TEST_SLOW_HOST() ? 750 : 250);
    assert(d < ms(TEST_SLOW_HOST() ? 500 : 200));  // within 200ms
}

int main(int, char**)
{
    m.lock();
    std::thread t = support::make_test_thread(f);
    std::this_thread::sleep_for(ms(TEST_SLOW_HOST() ? 750 : 250));
    m.unlock();
    t.join();

  return 0;
}
