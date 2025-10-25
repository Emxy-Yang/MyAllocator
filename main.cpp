#include "src.hpp"

#include <iostream>

int usedBlocks = 0;
int usedSpace = 0;
int maxUsedSpace = 0;

int* getNewBlock(int n) {
    if (n <= 0) return nullptr;
    usedBlocks += n;
    return new int[n * 4096 / sizeof(int)];
}

void freeBlock(const int* block, int n) {
    if (block == nullptr || n <= 0) return;
    delete[] block;
}

int* allocate(Allocator& allocator, int n) {
    usedSpace += n;
    if (usedSpace > maxUsedSpace) maxUsedSpace = usedSpace;
    return allocator.allocate(n);
}

void deallocate(Allocator& allocator, int* pointer, int n) {
    usedSpace -= n;
    allocator.deallocate(pointer, n);
}

bool check(Allocator& allocator) {
    return (usedBlocks - 1) * 4096 / sizeof(int) / 2 < maxUsedSpace;
}

int main() {
    Allocator allocator;
    int* a = allocate(allocator, 100);
    for (int i = 0; i < 100; ++i) {
        a[i] = i;
    }
    int* b = allocate(allocator, 40960 / sizeof(int));
    for (int i = 0; i < 40960 / sizeof(int); ++i) {
        b[i] = i;
    }
    deallocate(allocator, b, 40960 / sizeof(int));
    b = allocate(allocator, 4096 / sizeof(int)); // will not allocate a new block
    for (int i = 0; i < 4096 / sizeof(int); ++i) {
        b[i] = i;
    }
    deallocate(allocator, b, 4096 / sizeof(int));
    b = allocate(allocator, 4096 / sizeof(int));
    for (int i = 0; i < 4096 / sizeof(int); ++i) {
        b[i] = i;
    }
    for (int i = 0; i < 100; ++i) {
        if (a[i] != i) {
            std::cout << "Error: Data integrity is not okay!" << std::endl;
            return 0;
        }
    }
    for (int i = 0; i < 4096 / sizeof(int); ++i) {
        if (b[i] != i) {
            std::cout << "Error: Data integrity is not okay!" << std::endl;
            return 0;
        }
    }
    if (!check(allocator)) {
        std::cout << "Using too much space" << std::endl;
    } else {
        std::cout << "Passed" << std::endl;
    }
}