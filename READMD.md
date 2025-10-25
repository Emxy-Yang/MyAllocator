## MyAllocator : 一个基于4096byte倍数的块分配内存的 int Allocator

### 实现功能：
1、基于块的内存分配和释放  
 ```
 int* allocate(int n);  //n is the num of int needed
 
 //finished by the following function
 int* set_new_block(int *start , int capacity , int n) ;
 ```
2、对块中某一段内存释放以及对空内存进行管理
```
void deallocate(int* pointer, int n) ;

//Sort and Merge memory by following:
void Merge(block & temp);
```

### 辅助函数（由助教提供）
* 内存分配函数：
````
int* getNewBlock(int n) {
    if (n <= 0) return nullptr;
    usedBlocks += n;
    return new int[n * 4096 / sizeof(int)];
}
````

* 内存释放函数：
````
void freeBlock(const int* block, int n) {
    if (block == nullptr || n <= 0) return;
    delete[] block;
}
````


> comment：在某种意义上实现了模块化的内存分配，并且与 `new` 和 `delete` 进行了分离，增强了安全性（应该吧doge）