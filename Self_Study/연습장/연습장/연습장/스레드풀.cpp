#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    void enqueue(std::function<void()> task);
    void waitForCompletion(); // ��� �۾��� ���� ������ ���

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    std::condition_variable completionCondition;
    bool stop;
    size_t activeTasks = 0; // ���� ���� �۾� ���� ����

    void workerThread();
};

// ������ Ǯ ������
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

// �۾� �߰�
void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(std::move(task));
        activeTasks++; // �۾� ���� ����
    }
    condition.notify_one(); // ��� ���� ������ �����
}

// ������ ���� �Լ�
void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });

            if (stop && tasks.empty()) return;

            task = std::move(tasks.front());
            tasks.pop();
        }

        task(); // �۾� ����

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            activeTasks--; // �۾� �Ϸ� �� ����
            if (activeTasks == 0) {
                completionCondition.notify_all(); // ��� �۾� �Ϸ� �˸�
            }
        }
    }
}

// ��� �۾��� ���� ������ ���
void ThreadPool::waitForCompletion() {
    std::unique_lock<std::mutex> lock(queueMutex);
    completionCondition.wait(lock, [this] { return activeTasks == 0; });
}

// ������ Ǯ �Ҹ���: ��� ������ ����
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all(); // ��� �����带 ����

    for (std::thread& worker : workers) {
        worker.join(); // ��� �����尡 ���� ������ ���
    }
}

// ��� ����
int main() {
    ThreadPool pool(10); // 4���� ������ ����

    for (int i = 1; i <= 10; ++i) {
        pool.enqueue([i] {
            std::cout << "Task " << i << " is running on thread "
                << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            });
    }

    pool.waitForCompletion(); // ��� �۾��� ���� ������ ���

    std::cout << "All tasks completed!" << std::endl;
    return 0;
}
