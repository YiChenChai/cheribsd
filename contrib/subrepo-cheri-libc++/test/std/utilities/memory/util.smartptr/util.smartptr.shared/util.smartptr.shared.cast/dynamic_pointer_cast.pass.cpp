//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: libcpp-no-rtti

// <memory>

// shared_ptr

// template<class T, class U> shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r) noexcept;
// template<class T, class U> shared_ptr<T> dynamic_pointer_cast(shared_ptr<U>&& r) noexcept;

// UNSUPPORTED: no-rtti

#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

#include "test_macros.h"

struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};

int B::count = 0;

struct A
    : public B
{
    static int count;

    A() {++count;}
    A(const A& other) : B(other) {++count;}
    ~A() {--count;}
};

int A::count = 0;

int main(int, char**)
{
    {
        const std::shared_ptr<B> pB(new A);
        ASSERT_NOEXCEPT(std::dynamic_pointer_cast<A>(pB));
        std::shared_ptr<A> pA = std::dynamic_pointer_cast<A>(pB);
        assert(pA.get() == pB.get());
        assert(!pB.owner_before(pA) && !pA.owner_before(pB));
    }
    {
        const std::shared_ptr<B> pB(new B);
        std::shared_ptr<A> pA = std::dynamic_pointer_cast<A>(pB);
        assert(pA.get() == 0);
        assert(pA.use_count() == 0);
    }
#if TEST_STD_VER > 14
    {
      const std::shared_ptr<B[8]> pB(new B[8]);
      std::shared_ptr<A[8]> pA = std::dynamic_pointer_cast<A[8]>(pB);
      assert(pA.get() == 0);
      assert(pA.use_count() == 0);
    }
#endif // TEST_STD_VER > 14
#if TEST_STD_VER > 20
    {
      A* pA_raw = new A;
      std::shared_ptr<B> pB(pA_raw);
      ASSERT_NOEXCEPT(std::dynamic_pointer_cast<A>(std::move(pB)));
      std::shared_ptr<A> pA = std::dynamic_pointer_cast<A>(std::move(pB));
      assert(pB.get() == nullptr);
      assert(pA.get() == pA_raw);
      assert(pA.use_count() == 1);
    }
#endif // TEST_STD_VER > 20

    return 0;
}
