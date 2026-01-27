#include "thread_queue.h"

using namespace std;

file_copy::file_copy(std::filesystem::path source,std::filesystem::path destination,std::filesystem::copy_options c){
    this->source=source;
    this->destination=destination;
    this->c=c;
}

file_copy::file_copy() {};


void thread_queue::push_to_queue(file_copy f){
    lock_guard<mutex> lock(queue_operation_semaphore);
    file_queue.push(f);
    cv.notify_one();
}

bool thread_queue::pop_from_queue(file_copy& out){
    std::unique_lock<std::mutex> lock(queue_operation_semaphore);

    cv.wait(lock, [&] {
        return !file_queue.empty() || close;
    });

    if (file_queue.empty() && close) {
        return false;
    }

    out = file_queue.front();
    file_queue.pop();
    return true;
}

thread_queue::thread_queue()
{
}

thread_queue::~thread_queue() 
{
}

void thread_queue::shutdown() {
    {
        lock_guard<mutex> lock(queue_operation_semaphore);
        close = 1;
    }
    cv.notify_all();
}
