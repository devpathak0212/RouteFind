#include "MinHeap.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <random>

int main() {
    // Basic push/pop ordering
    {
        MinHeap h;
        h.push(5, 100);
        h.push(2, 200);
        h.push(8, 300);
        h.push(1, 400);

        assert(h.top().first == 1);
        assert(h.pop() == std::make_pair(1, 400));
        assert(h.pop() == std::make_pair(2, 200));
        assert(h.pop() == std::make_pair(5, 100));
        assert(h.pop() == std::make_pair(8, 300));
        assert(h.empty());
    }

    // Duplicate priorities should not break ordering
    {
        MinHeap h;
        h.push(3, 1);
        h.push(3, 2);
        h.push(3, 3);
        assert(h.pop().first == 3);
        assert(h.pop().first == 3);
        assert(h.pop().first == 3);
        assert(h.empty());
    }

    // Empty heap should throw, not crash or return garbage
    {
        MinHeap h;
        bool threwOnPop = false, threwOnTop = false;
        try { h.pop(); } catch (const std::out_of_range&) { threwOnPop = true; }
        try { h.top(); } catch (const std::out_of_range&) { threwOnTop = true; }
        assert(threwOnPop && threwOnTop);
    }

    // Stress test: push random values, pop them all out, confirm sorted order
    // matches std::sort on the same data (cross-check against a known-correct method)
    {
        MinHeap h;
        std::vector<int> priorities;
        std::mt19937 rng(42); // fixed seed for reproducibility
        std::uniform_int_distribution<int> dist(0, 10000);

        for (int i = 0; i < 1000; ++i) {
            int p = dist(rng);
            priorities.push_back(p);
            h.push(p, i);
        }

        std::sort(priorities.begin(), priorities.end());

        for (int expected : priorities) {
            assert(h.pop().first == expected);
        }
        assert(h.empty());
    }

    std::cout << "All MinHeap tests passed!" << std::endl;
    return 0;
}