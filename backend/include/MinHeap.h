#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>
#include <utility>
#include <stdexcept>

// A generic binary min-heap (priority queue) built from scratch.
// Stores {priority, id} pairs and always lets you retrieve the pair
// with the smallest priority in O(1), with O(log n) insert/remove.
//
// The heap itself knows nothing about grids, cells, or paths — it is
// a purely general-purpose "always give me the smallest" tool. The
// caller (e.g. Dijkstra/A*) decides what "priority" and "id" mean.
class MinHeap {
public:
    void push(int priority, int id);
    std::pair<int, int> pop();
    std::pair<int, int> top() const;

    bool empty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }

private:
    std::vector<std::pair<int, int>> heap;

    void siftUp(int index);
    void siftDown(int index);

    int parent(int i) const { return (i - 1) / 2; }
    int left(int i) const { return 2 * i + 1; }
    int right(int i) const { return 2 * i + 2; }
};

#endif