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
    void waitForCompletion(); // 모든 작업이 끝날 때까지 대기

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    std::condition_variable completionCondition;
    bool stop;
    size_t activeTasks = 0; // 실행 중인 작업 개수 추적

    void workerThread();
};

// 스레드 풀 생성자
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

// 작업 추가
void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(std::move(task));
        activeTasks++; // 작업 개수 증가
    }
    condition.notify_one(); // 대기 중인 스레드 깨우기
}

// 스레드 실행 함수
void ThreadPool::workerThread() 
{
    while (true) 
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });

            if (stop && tasks.empty()) return;

            task = std::move(tasks.front());
            tasks.pop();
        }

        task(); // 작업 실행

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            activeTasks--; // 작업 완료 시 감소
            if (activeTasks == 0) {
                completionCondition.notify_all(); // 모든 작업 완료 알림
            }
        }
    }
}

// 모든 작업이 끝날 때까지 대기
void ThreadPool::waitForCompletion() {
    std::unique_lock<std::mutex> lock(queueMutex);
    completionCondition.wait(lock, [this] { return activeTasks == 0; });
}

// 스레드 풀 소멸자: 모든 스레드 종료
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all(); // 모든 스레드를 깨움

    for (std::thread& worker : workers) {
        worker.join(); // 모든 스레드가 끝날 때까지 대기
    }
}

// 사용 예제
int main() {
    ThreadPool pool(10); // 4개의 스레드 생성

    for (int i = 1; i <= 10; ++i) {
        pool.enqueue([i] {
            std::cout << "Task " << i << " is running on thread "
                << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            });
    }

    pool.waitForCompletion(); // 모든 작업이 끝날 때까지 대기

    std::cout << "All tasks completed!" << std::endl;
    return 0;
}
