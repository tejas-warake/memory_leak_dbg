#include "memory_tracker.hpp"

#define new new(__FILE__, __LINE__)

int main() {
    int* test = new int;
    delete[] test;

    int* leak = new int[100];

    finalizeMemoryTracker();

    return 0;
}
