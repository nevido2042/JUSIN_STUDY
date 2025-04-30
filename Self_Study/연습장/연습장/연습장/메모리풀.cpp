#include <iostream>
#include <vector>

class MemoryPool {
private:
    struct Block {
        Block* next; // ���� ����� ����Ű�� ������
    };

    Block* freeList; // ��� ������ ��� ����Ʈ
    std::vector<void*> allocatedChunks; // ���� �Ҵ�� ûũ ����
    size_t blockSize;
    size_t chunkSize;

public:
    MemoryPool(size_t blockSize, size_t chunkSize)
        : blockSize(blockSize), chunkSize(chunkSize), freeList(nullptr) {
        allocateChunk(); // �ʱ� ��� �Ҵ�
    }

    ~MemoryPool() {
        for (void* chunk : allocatedChunks) {
            free(chunk);
        }
    }

    void* allocate() {
        if (!freeList) {
            allocateChunk(); // ����� �����ϸ� ���ο� ûũ �Ҵ�
        }

        Block* block = freeList;
        freeList = freeList->next; // ���� ������� �̵�
        return block;
    }

    void deallocate(void* ptr) {
        Block* block = static_cast<Block*>(ptr);
        block->next = freeList; // ���� ����Ʈ�� ��ȯ
        freeList = block;
    }

private:
    void allocateChunk() {
        size_t totalSize = blockSize * chunkSize;
        void* chunk = malloc(totalSize);
        allocatedChunks.push_back(chunk);

        // ����� ��ũ�� ����Ʈ�� ����
        char* start = static_cast<char*>(chunk);
        for (size_t i = 0; i < chunkSize; ++i) {
            Block* block = reinterpret_cast<Block*>(start + i * blockSize);
            block->next = freeList;
            freeList = block;
        }
    }
};

//  ��� ����
struct Test {
    int a, b;
    float c;
};

int main() {
    MemoryPool pool(sizeof(Test), 10); // Test ũ���� ����� 10���� ����

    Test* t1 = static_cast<Test*>(pool.allocate());
    t1->a = 1;
    t1->b = 2;
    t1->c = 3.14f;

    std::cout << "Test: " << t1->a << ", " << t1->b << ", " << t1->c << '\n';

    pool.deallocate(t1); // �޸� Ǯ�� ��ȯ

    return 0;
}
