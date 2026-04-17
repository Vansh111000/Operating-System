#include <iostream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <mutex>

using namespace std;

// Named semaphores
sem_t *read_mutex;
sem_t *wrt;
sem_t *queue_mutex;

int read_count = 0;

mutex print_mutex;

// Tracking
vector<string> arrival_order;
vector<string> execution_log;
vector<string> finish_order;

void random_delay() {
    usleep((rand() % 800 + 200) * 1000); 
}

void log_event(string msg, vector<string> &log_vec) {
    lock_guard<mutex> lock(print_mutex);
    log_vec.push_back(msg);
    cout << msg << endl;   
}

void reader(int id) {
    random_delay();

    string name = "Reader " + to_string(id);
    log_event(name + " ARRIVED", arrival_order);

    log_event(name + " WAITING (blocked if writer active)", execution_log);
    sem_wait(queue_mutex);
    sem_wait(read_mutex);

    read_count++;
    if (read_count == 1) {
        sem_wait(wrt);
    }

    sem_post(read_mutex);
    sem_post(queue_mutex);

    log_event(name + " >>> READING (shared access)", execution_log);
    random_delay();

    sem_wait(read_mutex);
    read_count--;

    if (read_count == 0) {
        sem_post(wrt);
    }

    sem_post(read_mutex);
    log_event(name + " FINISHED", finish_order);
}

void writer(int id) {
    random_delay();

    string name = "Writer " + to_string(id);
    log_event(name + " ARRIVED", arrival_order);

    log_event(name + " WAITING (will block readers)", execution_log);
    sem_wait(queue_mutex);
    sem_wait(wrt);

    log_event(name + " >>> WRITING (exclusive access, others blocked)", execution_log);
    random_delay();

    log_event(name + " FINISHED", finish_order);

    sem_post(wrt);
    sem_post(queue_mutex);
}

int main() {
    srand(time(NULL));
    // Create semaphores
    read_mutex = sem_open("/read_mutex", O_CREAT, 0644, 1);
    wrt = sem_open("/wrt", O_CREAT, 0644, 1);
    queue_mutex = sem_open("/queue_mutex", O_CREAT, 0644, 1);

    thread readers[5], writers[5];

    // Create threads
    for (int i = 0; i < 5; i++) {
        readers[i] = thread(reader, i + 1);
        writers[i] = thread(writer, i + 1);
    }
    for (int i = 0; i < 5; i++) {
        readers[i].join();
        writers[i].join();
    }

    cout << "\n========== ARRIVAL ORDER ==========\n";
    for (auto &s : arrival_order) cout << s << endl;

    cout << "\n========== EXECUTION ==========\n";
    for (auto &s : execution_log) cout << s << endl;

    cout << "\n========== FINISH ORDER ==========\n";
    for (auto &s : finish_order) cout << s << endl;

    // Cleanup
    sem_close(read_mutex);
    sem_close(wrt);
    sem_close(queue_mutex);

    sem_unlink("/read_mutex");
    sem_unlink("/wrt");
    sem_unlink("/queue_mutex");

    return 0;
}