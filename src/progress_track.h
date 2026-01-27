#include<mutex>
#include<atomic>
#include<iostream>

class progress
{

    std::atomic<int> copy_success_counter, copy_fail_counter;
    std::atomic<uintmax_t> byte_counter;
    bool close =  false;

    std::atomic_int workers_number_buffer;

public :

    progress();

    void close_progress_tracker();

    void track_progress();

    void increment_byte_counter(uintmax_t increment);

    void increment_success_counter();

    void increment_failed_counter();

    void incrment_worker_buffer();
};




