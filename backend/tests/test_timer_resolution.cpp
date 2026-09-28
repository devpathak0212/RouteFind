#include "Timer.h"
#include <iostream>
#include <cassert>
#include <set>

int main() {
    // Take many back-to-back readings. A fine-grained clock produces many
    // distinct values; a coarse clock (1 ms or 15 ms ticks) produces only a
    // handful, which is what made fast solves randomly show 0.000 ms.
    Timer t;
    t.start();
    std::set<double> distinct;
    for (int i = 0; i < 100000; ++i) {
        distinct.insert(t.elapsedMs());
    }

    std::cout << "  distinct readings: " << distinct.size() << std::endl;
    assert(distinct.size() > 100);

    std::cout << "Timer resolution test passed!" << std::endl;
    return 0;
}
