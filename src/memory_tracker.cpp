#include "memory_tracker.hpp"
#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <mutex>

std::mutex memoryMutex;

struct mem_alloc_struct {
    size_t size;
    const char* file;
    int line;
};

std::unordered_map<void*, mem_alloc_struct> memAllocMap;

void* operator new(size_t size, const char* file, int line) {
    std::lock_guard<std::mutex> guard(memoryMutex);

    void* ptr = std::malloc(size);
    if (!ptr) throw std::bad_alloc();

    memAllocMap[ptr] = {size, file, line};  // false means it's a scalar new
    std::cout << "Allocated " << size << " bytes at " << ptr << " (File: " << file << ", Line: " << line << ")\n";
    return ptr;
}

void* operator new[](size_t size, const char* file, int line) {
    std::lock_guard<std::mutex> guard(memoryMutex);

    void* ptr = std::malloc(size);
    if (!ptr) throw std::bad_alloc();

    memAllocMap[ptr] = {size, file, line};  
    std::cout << "Allocated " << size << " bytes at " << ptr << " (File: " << file << ", Line: " << line << ")\n";
    return ptr;
}

// Overload the global delete operator
// void operator delete(void* ptr) noexcept {
//     std::lock_guard<std::mutex> guard(memoryMutex);

//     auto it = memAllocMap.find(ptr);
//     if (it != memAllocMap.end()) {
//         std::cout << "Deallocated " << it->second.size << " bytes from " << ptr << " (File: " << it->second.file << ", Line: " << it->second.line << ")\n";
//         memAllocMap.erase(it);
//     } else {
//         std::cout << "Deallocated unknown memory " << ptr << "\n";
//     }
//     std::free(ptr);
// }

void operator delete[](void* ptr) noexcept {
    std::lock_guard<std::mutex> guard(memoryMutex);

    auto it = memAllocMap.find(ptr);
    if (it != memAllocMap.end()) {
        std::cout << "Deallocated " << it->second.size << " bytes from " << ptr << " (File: " << it->second.file << ", Line: " << it->second.line << ")\n";
        memAllocMap.erase(it);
    } else {
        std::cout << "Deallocated unknown memory " << ptr << "\n";
    }
    std::free(ptr);
}

void reportLeaks() {
    std::lock_guard<std::mutex> guard(memoryMutex);

    if (!memAllocMap.empty()) {
        std::cout << "\nMemory leaks detected:\n";
        for (const auto& alloc : memAllocMap) {
            std::cout << "Leaked " << alloc.second.size << " bytes at " << alloc.first 
                      << " (File: " << alloc.second.file << ", Line: " << alloc.second.line << ")\n";
        }
    } else {
        std::cout << "No memory leaks detected.\n";
    }
}

void initializeMemoryTracker() {
}

void finalizeMemoryTracker() {
    reportLeaks();
}
