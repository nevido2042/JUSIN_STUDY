#include <iostream>
#include <vector>

class MemoryPool {
private:
    struct Block {
        Block* next; // 다음 블록을 가리키는 포인터
    };

    Block* freeList; // 사용 가능한 블록 리스트
    std::vector<void*> allocatedChunks; // 동적 할당된 청크 저장
    size_t blockSize;
    size_t chunkSize;

public:
    MemoryPool(size_t blockSize, size_t chunkSize)
        : blockSize(blockSize), chunkSize(chunkSize), freeList(nullptr) {
        allocateChunk(); // 초기 블록 할당
    }

    ~MemoryPool() {
        for (void* chunk : allocatedChunks) {
            free(chunk);
        }
    }

    void* allocate() {
        if (!freeList) {
            allocateChunk(); // 블록이 부족하면 새로운 청크 할당
        }

        Block* block = freeList;
        freeList = freeList->next; // 다음 블록으로 이동
        return block;
    }

    void deallocate(void* ptr) {
        Block* block = static_cast<Block*>(ptr);
        block->next = freeList; // 프리 리스트에 반환
        freeList = block;
    }

private:
    void allocateChunk() {
        size_t totalSize = blockSize * chunkSize;
        void* chunk = malloc(totalSize);
        allocatedChunks.push_back(chunk);

        // 블록을 링크드 리스트로 연결
        char* start = static_cast<char*>(chunk);
        for (size_t i = 0; i < chunkSize; ++i) {
            Block* block = reinterpret_cast<Block*>(start + i * blockSize);
            block->next = freeList;
            freeList = block;
        }
    }
};

//  사용 예제
struct Test {
    int a, b;
    float c;
};

int main() {
    MemoryPool pool(sizeof(Test), 10); // Test 크기의 블록을 10개씩 관리

    Test* t1 = static_cast<Test*>(pool.allocate());
    t1->a = 1;
    t1->b = 2;
    t1->c = 3.14f;

    std::cout << "Test: " << t1->a << ", " << t1->b << ", " << t1->c << '\n';

    pool.deallocate(t1); // 메모리 풀에 반환

    return 0;
}
