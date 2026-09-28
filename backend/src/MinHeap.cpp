#include "MinHeap.h"
#include <algorithm>

void MinHeap::push(int priority, int id) {
    heap.push_back({priority, id});
    siftUp(static_cast<int>(heap.size()) - 1);
}

std::pair<int, int> MinHeap::pop() {
    if (heap.empty()) throw std::out_of_range("pop: heap is empty");
    std::pair<int, int> minItem = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) siftDown(0);
    return minItem;
}

std::pair<int, int> MinHeap::top() const {
    if (heap.empty()) throw std::out_of_range("top: heap is empty");
    return heap[0];
}

void MinHeap::siftUp(int index) {
    while (index > 0 && heap[index].first < heap[parent(index)].first) {
        std::swap(heap[index], heap[parent(index)]);
        index = parent(index);
    }
}

void MinHeap::siftDown(int index) {
    int size = static_cast<int>(heap.size());
    while (true) {
        int smallest = index;
        int l = left(index), r = right(index);
        if (l < size && heap[l].first < heap[smallest].first) smallest = l;
        if (r < size && heap[r].first < heap[smallest].first) smallest = r;
        if (smallest == index) break;
        std::swap(heap[index], heap[smallest]);
        index = smallest;
    }
}