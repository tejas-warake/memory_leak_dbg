#ifndef MEMORY_TRACKER_HPP
#define MEMORY_TRACKER_HPP

#include <cstddef>

void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size, const char* file, int line);
void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;
void initializeMemoryTracker();
void finalizeMemoryTracker();
void reportLeaks();

#endif