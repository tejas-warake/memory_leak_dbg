#include "memory_tracker.hpp"

#ifdef MEM_DBG
#define new new(__FILE__, __LINE__)
#endif

int main() {
    int* test = new int;
    delete[] test;

    int* leak = new int[100];

    finalizeMemoryTracker();

    return 0;
}
