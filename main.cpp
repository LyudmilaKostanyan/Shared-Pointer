#include <iostream>
#include <cassert>
#include "SharedPtr.hpp"

void test_basic_reference_counting() {
    std::cout << "== Running Basic Reference Counting Test ==\n";

    SharedPtr<int> p(new int(123));
    std::cout << "  After creation: p.use_count() = " << p.use_count() << " (expected 1)\n";
    assert(p.use_count() == 1);

    SharedPtr<int> q(p);
    std::cout << "  After copy-ctor: p.use_count() = " << p.use_count()
              << ", q.use_count() = " << q.use_count() << " (expected 2, 2)\n";
    assert(p.use_count() == 2);
    assert(q.use_count() == 2);

    SharedPtr<int> r;
    r = p;
    std::cout << "  After assignment: p.use_count() = " << p.use_count()
              << ", r.use_count() = " << r.use_count() << " (expected 3, 3)\n";
    assert(p.use_count() == 3);
    assert(r.use_count() == 3);

    r.reset();
    std::cout << "  After r.reset(): r.use_count() = " << r.use_count()
              << ", p.use_count() = " << p.use_count() << " (expected 0, 2)\n";
    assert(r.use_count() == 0);
    assert(p.use_count() == 2);

    p.reset();
    q.reset();
    std::cout << "  After p.reset() & q.reset(): p.use_count() = " << p.use_count()
              << ", q.use_count() = " << q.use_count() << " (expected 0, 0)\n";
    assert(p.use_count() == 0);
    assert(q.use_count() == 0);

    std::cout << "[PASS] Basic Reference Counting\n\n";
}

void test_self_assignment() {
    std::cout << "== Running Self-Assignment Test ==\n";

    SharedPtr<int> a(new int(5));
    std::cout << "  After creation: a.use_count() = " << a.use_count() << " (expected 1)\n";
    assert(a.use_count() == 1);

    a = a;
    std::cout << "  After self-assignment: a.use_count() = " << a.use_count() << " (expected 1)\n";
    assert(a.use_count() == 1);

    std::cout << "[PASS] Self-Assignment\n\n";
}

void test_swap() {
    std::cout << "== Running Swap Test ==\n";

    SharedPtr<int> a(new int(1));
    SharedPtr<int> b(new int(2));
    std::cout << "  Before swap: a.use_count() = " << a.use_count()
              << ", b.use_count() = " << b.use_count() << " (expected 1, 1)\n";
    assert(a.use_count() == 1 && b.use_count() == 1);

    a.swap(b);
    std::cout << "  After swap: a.use_count() = " << a.use_count()
              << ", b.use_count() = " << b.use_count() << " (expected 1, 1)\n";
    assert(a.use_count() == 1);
    assert(b.use_count() == 1);

    std::cout << "[PASS] Swap Operation\n\n";
}

int main() {
    std::cout << "=== SharedPtr Unit Tests ===\n\n";

    test_basic_reference_counting();
    test_self_assignment();
    test_swap();

    std::cout << "=== All tests completed successfully ===\n";
    return 0;
}
