#define _POSIX_C_SOURCE 200809L

#include "pool.h"

#include <pthread.h>
#include <time.h>
#include <atomic>
#include <set>
#include <iostream>
#include <sstream>
#include <vector>

namespace {

std::string N(int i) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

std::string T(std::string label, int i) {
    std::stringstream ss;
    ss << label << "#" << i;
    return ss.str();
}

struct BarrierTask : Task {
    pthread_barrier_t *barrier_;
    std::atomic<int> *counter_;

    BarrierTask(pthread_barrier_t *barrier, std::atomic<int> *counter)
        : barrier_(barrier), counter_(counter) {};

    void Run() override {
        pthread_barrier_wait(barrier_);
        if (counter_) counter_->fetch_add(1);
    }
};

struct RecordIncrementTask : Task {
    std::atomic<int> *counter_;
    std::atomic<int> *destroy_counter_;
    int *record_value_;

    RecordIncrementTask(std::atomic<int> *counter, int *record_value) :
        counter_(counter), destroy_counter_(0), record_value_(record_value) {}
    RecordIncrementTask(std::atomic<int> *counter, std::atomic<int>* destroy_counter, int *record_value) :
        counter_(counter), destroy_counter_(destroy_counter), record_value_(record_value) {}

    void Run() override {
        *record_value_ = counter_->fetch_add(1);
    }

    ~RecordIncrementTask() {
        if (destroy_counter_) {
            destroy_counter_->fetch_add(1);
        }
    }
};

struct WaitForTaskTask : Task {
    ThreadPool *pool_;
    std::string to_wait_for_;
    bool *done_ptr_;

    WaitForTaskTask(ThreadPool *pool, const std::string &to_wait_for, bool* done_ptr) :
        pool_(pool), to_wait_for_(to_wait_for), done_ptr_(done_ptr) {}

    void Run() override {
        pool_->WaitForTask(to_wait_for_);
        *done_ptr_ = true;
    }
};

struct SubmitTaskTask : Task {
    ThreadPool *pool_;
    std::string new_task_name_;
    Task *new_task_;

    SubmitTaskTask(ThreadPool *pool, const std::string &new_task_name, Task* new_task) :
        pool_(pool), new_task_name_(new_task_name), new_task_(new_task) {}

    void Run() override {
        pool_->SubmitTask(new_task_name_, new_task_);
        new_task_ = 0;
    }
};

struct EmptyTask : Task {
    void Run() override {}
};

struct CheckDestructionTask : Task {
    bool *destroyed_ptr_;

    CheckDestructionTask(bool *destroyed_ptr) : destroyed_ptr_(destroyed_ptr) {}

    void Run() override {}

    ~CheckDestructionTask() {
        *destroyed_ptr_ = true;
    }
};

struct CancelTaskTask : Task {
    CancelTaskTask(ThreadPool *pool, const std::string &to_cancel_name,
                   bool *cancel_result_ptr, bool *did_cancel_ptr) :
        pool_(pool), to_cancel_name_(to_cancel_name),
        cancel_result_ptr_(cancel_result_ptr), did_cancel_ptr_(did_cancel_ptr) {}

    void Run() override {
        *cancel_result_ptr_ = pool_->CancelTask(to_cancel_name_);
        *did_cancel_ptr_ = true;
    }

    ThreadPool *pool_;
    std::string to_cancel_name_;
    bool *cancel_result_ptr_;
    bool *did_cancel_ptr_;
};

struct TaskChainInfo {
    ThreadPool *pool;
    std::string label_base;
    pthread_mutex_t lock;
    bool enabled;
    std::atomic<int> next_task_number;
    std::set<int> submitted_but_unrun;

    void DoSubmit(int from_id);

    void Disable() {
        pthread_mutex_lock(&lock);
        enabled = false;
        pthread_mutex_unlock(&lock);
    }

    TaskChainInfo() {
        pthread_mutex_init(&lock, NULL);
    }
    ~TaskChainInfo() {
        pthread_mutex_destroy(&lock);
    }
};

struct ChainSubmitTask : Task {
    ChainSubmitTask(TaskChainInfo *info, int id) : info_(info), id_(id) {}

    void Run() override {
        info_->DoSubmit(id_);
    }
   
    TaskChainInfo *info_;
    int id_;
};

void TaskChainInfo::DoSubmit(int from_id) {
    pthread_mutex_lock(&lock);
    if (enabled) {
        int new_task = next_task_number.fetch_add(1);
        if (new_task > 1000000) {
            std::cerr << "refusing to create more than 1 million tasks\n";
            std::cerr << "(either Pause() not working or some bad luck?)\n";
            abort();
        }
        pool->SubmitTask(
            T(label_base, new_task),
            new ChainSubmitTask(this, new_task)
        );
        submitted_but_unrun.insert(new_task);
    }
    submitted_but_unrun.erase(from_id);
    pthread_mutex_unlock(&lock);
}

std::string current_test{"<none>"};
// bool DEBUG = false;
int passed_subtests = 0;
int failed_subtests = 0;
int passed_tests = 0;
int failed_tests = 0;

std::vector<std::string> failed_test_groups;

void START_TEST(std::string name) {
    if (current_test != "<none>") {
        std::cerr << "test mismatch for " << name << " versus " << current_test << std::endl;
        abort();
    }
    passed_subtests = failed_subtests = 0;
    current_test = name;
    std::cout << "STARTING test group: " << name << std::endl;
}

void END_TEST(std::string name) {
    if (current_test != name) {
        std::cerr << "test mismatch for " << name << " versus " << current_test << std::endl;
        abort();
    }
    current_test = "<none>";
    if (failed_subtests > 0) {
        ++failed_tests;
        std::cout << "FAILED test group: " << name << std::endl;
        failed_test_groups.push_back(name);
    } else {
        ++passed_tests;
        std::cout << "PASSED test group: " << name << std::endl;
    }
}

void CHECK(std::string description, bool value) {
    if (value) {
        std::cout << "PASSED: " << current_test << ": " << description << std::endl;
        ++passed_subtests;
    } else {
        std::cout << "FAILED: " << current_test << ": " << description << std::endl;
        ++failed_subtests;
    }
}

void CHECK_QUIET(std::string description, bool value) {
    if (value) {
        ++passed_subtests;
    } else {
        std::cout << "FAILED: " << current_test << ": " << description << std::endl;
        ++failed_subtests;
    }
}


void submit_barrier_set(ThreadPool *pool, int thread_count, pthread_barrier_t *barrier, std::atomic<int> *check_count, std::string label = "barrier") {
    for (int i = 0; i < thread_count; ++i) {
        pool->SubmitTask(T(label, i), new BarrierTask(barrier, check_count));
    }
}

void cleanup_barrier_set(ThreadPool *pool, int thread_count, std::atomic<int> *check_count, std::string label = "barrier") {
    for (int i = 0; i < thread_count; ++i) {
        pool->WaitForTask(T(label, i));
    }
    CHECK("tasks waiting on barrier (used to make sure some tasks are submitted after other tasks run in parallel) run correct number of times", check_count->load() == thread_count);
}

void test_cancel_future(int thread_count, int tasks) {
    std::atomic<int> before_cancel_check_count{0}, after_cancel_check_count{0};
    const std::string description =
        "cancelling " + N(tasks) +
        " tasks from tasks running earlier with " +
        N(thread_count) + " threads";
    START_TEST(description);
    ThreadPool pool{thread_count};
    pthread_barrier_t before_cancel_barrier, after_cancel_barrier;
    pthread_barrier_init(&before_cancel_barrier, NULL, thread_count+1);
    pthread_barrier_init(&after_cancel_barrier, NULL, thread_count+1);

    submit_barrier_set(&pool, thread_count, &before_cancel_barrier, &before_cancel_check_count, "before-cancel");

    bool *cancel_results = new bool[tasks];
    bool *did_cancel_results = new bool[tasks];
    bool *destroy_results = new bool[tasks];
    for (int i = 0; i < tasks; ++i) {
        cancel_results[i] = false;
        pool.SubmitTask(T("cancel", i),
            new CancelTaskTask(&pool, T("empty", i), &cancel_results[i], &did_cancel_results[i])
        );
    }

    submit_barrier_set(&pool, thread_count, &after_cancel_barrier, &after_cancel_check_count, "after-cancel");

    for (int i = 0; i < tasks; ++i) {
        destroy_results[i] = false;
        pool.SubmitTask(T("empty", i), new CheckDestructionTask(&destroy_results[i]));
    }
    pthread_barrier_wait(&before_cancel_barrier);

    for (int i = 0; i < tasks; ++i) {
        pool.WaitForTask(T("cancel", i));
        CHECK_QUIET("cancelling task " + T("empty", i) + " attempted", did_cancel_results[i]);
        CHECK("cancelling task " + T("empty", i) + " succeeded", cancel_results[i]);
    }
    pthread_barrier_wait(&after_cancel_barrier);

    cleanup_barrier_set(&pool, thread_count, &before_cancel_check_count, "before-cancel");
    cleanup_barrier_set(&pool, thread_count, &after_cancel_check_count, "after-cancel");

    pool.Stop();
    for (int i = 0; i < tasks; ++i) {
        CHECK("task " + T("empty", i) + " destroyed", destroy_results[i]);
    }
    END_TEST(description);
    delete[] cancel_results;
    delete[] did_cancel_results;
    delete[] destroy_results;
    pthread_barrier_destroy(&before_cancel_barrier);
    pthread_barrier_destroy(&after_cancel_barrier);
}

void test_cancel_finished(int thread_count, int tasks) {
    std::atomic<int> barrier_check_count{0};
    const std::string description =
        "cancelling " + N(tasks) +
        " tasks which already ran from " +
        N(thread_count) + " threads";
    START_TEST(description);
    ThreadPool pool{thread_count};
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, thread_count+1);
    bool *cancel_results = new bool[tasks];
    bool *did_cancel_results = new bool[tasks];
    bool *destroy_results = new bool[tasks];
    for (int i = 0; i < tasks; ++i) {
        destroy_results[i] = false;
        pool.SubmitTask(T("empty", i), new CheckDestructionTask(&destroy_results[i]));
    }
    submit_barrier_set(&pool, thread_count, &barrier, &barrier_check_count);
    pthread_barrier_wait(&barrier);
    for (int i = 0; i < tasks; ++i) {
        cancel_results[i] = false;
        pool.SubmitTask(T("cancel", i),
            new CancelTaskTask(&pool, T("empty", i), &cancel_results[i], &did_cancel_results[i])
        );
    }
    for (int i = 0; i < tasks; ++i) {
        pool.WaitForTask(T("cancel", i));
        CHECK_QUIET("cancelling task " + T("empty", i) + " attempted", did_cancel_results[i]);
        CHECK("cancelling task " + T("empty", i) + " which should have already finished returned false", !cancel_results[i]);
    }
    cleanup_barrier_set(&pool, thread_count, &barrier_check_count);
    pool.Stop();
    for (int i = 0; i < tasks; ++i) {
        CHECK_QUIET("task " + T("empty", i) + " destroyed", destroy_results[i]);
    }
    END_TEST(description);
    delete[] cancel_results;
    delete[] did_cancel_results;
    delete[] destroy_results;
    pthread_barrier_destroy(&barrier);
}

void test_pause_probably_waits(int thread_count, int base_tasks, bool cancel_early = false) {
    ThreadPool pool{thread_count};
    const std::string description =
        "pausing stops chain of task-submitting tasks with " +
        N(thread_count) + " and " + N(base_tasks) + " inital tasks";
    START_TEST(description);
    std::cout << "before DoSubmit\n";
    TaskChainInfo chain;
    chain.pool = &pool;
    chain.label_base = "task-chain";
    chain.enabled = true;
    chain.next_task_number = 0;
    for (int i = 0; i < base_tasks; ++i) {
        chain.DoSubmit(-1);
    }
    pool.Pause();
    int expect_next_task = chain.next_task_number.load();
    CHECK(
        "no new tasks started after pausing (0)",
        expect_next_task == chain.next_task_number.load()
    );
    struct timespec twenty_millis;
    twenty_millis.tv_sec = 0;
    twenty_millis.tv_nsec = 20 * 1000 * 1000;
    nanosleep(&twenty_millis, NULL);
    CHECK(
        "no new tasks started after pausing (1)",
        expect_next_task == chain.next_task_number.load()
    );
    nanosleep(&twenty_millis, NULL);
    CHECK(
        "no new tasks started after pausing (2)",
        expect_next_task == chain.next_task_number.load()
    );
    nanosleep(&twenty_millis, NULL);
    CHECK(
        "no new tasks started after pausing (3)",
        expect_next_task == chain.next_task_number.load()
    );
    pthread_mutex_lock(&chain.lock);
    for (int i = 0; i < expect_next_task; ++i) {
        if (!chain.submitted_but_unrun.count(i)) {
            CHECK(
                "cancelling already run task should fail",
                !pool.CancelTask(T("task-chain", i))
            );
        } else if (cancel_early) {
            CHECK(
                "cancelling non-run task (that could not start due to being paused) should work",
                pool.CancelTask(T("task-chain", i))
            );
            chain.submitted_but_unrun.erase(i);
        }
    }
    std::set<int> was_unrun = chain.submitted_but_unrun;
    chain.enabled = false;
    pthread_mutex_unlock(&chain.lock);
    pool.Resume();
    for (auto i : was_unrun) {
        pool.CancelTask(T("task-chain", i));
    }
    pool.Stop();
    END_TEST(description);
}

}  // end unnamed namespace

int main(void) {
    test_cancel_future(1, 1);
    test_cancel_future(2, 2);
    test_cancel_future(1, 10);
    test_cancel_future(4, 10);
    
    test_cancel_finished(1, 1);
    test_cancel_finished(2, 1);
    test_cancel_finished(2, 2);
    test_cancel_finished(1, 10);
    test_cancel_finished(2, 10);
    test_cancel_finished(4, 10);

    test_pause_probably_waits(1, 1);
    test_pause_probably_waits(1, 2);
    test_pause_probably_waits(2, 2);
    test_pause_probably_waits(4, 100);
    
    std::cout << "--RESULT SUMMARY--\npassed " << passed_tests
              << " test groups and failed " << failed_tests << " test groups\n";
    if (failed_tests > 0) {
        std::cout << "Failed test groups:\n";
        for (auto group : failed_test_groups) {
            std::cout << "  " << group << "\n";
        }
    }
    return 0;
}
