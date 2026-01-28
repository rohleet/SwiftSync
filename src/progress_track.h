#include<mutex>
#include<atomic>
#include<iostream>
#include<condition_variable>

class progress
{

    std::atomic<int> copy_success_counter, copy_fail_counter;
    std::atomic<uintmax_t> byte_counter;
    std::atomic_bool close =  false;

    std::condition_variable p_cv;
    std::mutex progress_mutex;

    std::atomic_int workers_number_buffer;

public :

    progress();

    void close_progress_tracker();

    void track_progress();

    void increment_byte_counter(uintmax_t increment);

    void increment_success_counter();

    void increment_failed_counter();

    void incrment_worker_buffer();

    void decrement_worker_buffer();

    bool workers_empty();

    bool close_load();
};




