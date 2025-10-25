//
// Created by Yang Jibin on 2025/10/18.
//

#ifndef ALLOCATOR_SRC_HPP
#define ALLOCATOR_SRC_HPP

#include <functional>
#include<list>
#include<utility>
#include <vector>

/**
 * Get specified 4096 * n bytes from the memory.
 * @param n
 * @return the address of the block
 */
int* getNewBlock(int n);

/**
 * Free specified 4096 * n bytes from the memory.
 * @param block the pointer to the block
 * @param n
 */
void freeBlock(const int* block, int n);

// include whatever you want

class Allocator {
public:
    Allocator();
        // TODO： constructor 构造函数


    ~Allocator();
        // TODO： destructor 析构函数
        // Please note that even if some of data is not deallocated explicitly,
        // you should free them anyway.
        // 注意，即使此时仍然有没有被 deallocate 的数据，你也要将这些内存释放。


    /**
     * Allocate a sequence of memory space of n int.
     * @param n
     * @return the pointer to the allocated memory space
     */
    int* allocate(int n);
        // TODO： allocate member function
        // You should use the int* getNewBlock(int n) function
        // 请使用 int* getNewBlock(int n) 函数来获得可向外分配的内存
        // DO NOT return the memory resource that is got from new or malloc.
        // 请不要返回从 new 或者 malloc 获得的内存地址


    /**
     * Deallocate the memory that is allocated by the allocate member
     * function. If n is not the number that is called when allocating,
     * the behaviour is undefined.
     */
    void deallocate(int* pointer, int n) ;
        // TODO： deallocate the pointer

    int* set_new_block(int *start , int capacity , int n) ;

    bool IsCapable (int n) ;


private:
    static constexpr int Capacity = 4096/sizeof(int) ;
    class block {
    public:
        int *start_location_;
        int total_capacity;
        int used_size;
        int free_size;
        std::list<std::pair<int*,int>> free_space_;

        block (int *start , int total_capacity): start_location_(start) , total_capacity(total_capacity) {
            used_size = 0;
            free_size = total_capacity;
            //free_space_.emplace_back(std::make_pair(start,free_size));
        }
    };
    friend void Merge(block & temp);
    std::list<block> alloc_memory;
    std::vector<std::pair<int * , int *>> ptr;

    // Add whatever you want to add
};


inline int *Allocator::allocate(int n) {
    if (!IsCapable(n)) {
        return set_new_block(getNewBlock(n/Capacity+1) , (n/Capacity+1)*Capacity , n);
    }else {
        return ptr.back().second;
    }
}

inline Allocator::Allocator() {
    alloc_memory.clear();
    ptr.clear();
}

inline void Merge(Allocator::block & temp) {
    temp.free_space_.sort([](const std::pair<int*, int>& a, const std::pair<int*, int>& b) { return a.first < b.first; });
    auto it = temp.free_space_.begin();
    auto next = it;
    ++next;

    while (next != temp.free_space_.end()) {
        int* current_end = it->first + it->second;
        if (current_end == next->first) {
            it->second += next->second;
            next = temp.free_space_.erase(next);
        } else {
            ++it;
            ++next;
        }
    }
}


inline void Allocator::deallocate(int *pointer, int n) {

    for (auto block_it = alloc_memory.begin(); block_it != alloc_memory.end(); ++block_it) {
        auto &blk = *block_it;
        if (pointer >= blk.start_location_ && pointer < blk.start_location_ + blk.total_capacity) {
            blk.free_space_.emplace_back(pointer, n);
            blk.used_size -= n;
            blk.free_size += n;
            Merge(blk);

            if (blk.used_size == 0) {
                freeBlock(blk.start_location_, blk.total_capacity / Capacity);
                alloc_memory.erase(block_it);
            }
            break;
        }
    }

    for (auto it = ptr.begin(); it != ptr.end(); ++it) {
        if (it->second == pointer) {
            ptr.erase(it);
            break;
        }
    }
}

inline Allocator::~Allocator() {
    for (const auto & i : alloc_memory) {
        freeBlock(i.start_location_ , i.total_capacity/(Capacity));
    }
}

inline int *Allocator::set_new_block(int *start, int capacity , int n) {
    block temp(start,capacity);
    temp.free_size -= n;
    temp.used_size += n;
    if (temp.free_size > 0)
        temp.free_space_.emplace_back(start + n, temp.free_size);

    this->alloc_memory.push_back(temp);
    return start;
}

inline bool Allocator::IsCapable(int n) {
    if (alloc_memory.empty()) return false;
    for (auto &i : alloc_memory) {
        if (i.free_size < n) continue;
        else {
            for (auto j = i.free_space_.begin() ; j != i.free_space_.end() ; ++j) {
                if (j->second >= n) {
                    ptr.emplace_back(i.start_location_,(*j).first);
                    i.free_size -= n;
                    i.used_size += n;
                    std::pair<int*,int>temp = (*j);
                    if (j != i.free_space_.end()) {
                        (*j) = std::make_pair(temp.first + n , temp.second - n);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}







#endif //ALLOCATOR_SRC_HPP