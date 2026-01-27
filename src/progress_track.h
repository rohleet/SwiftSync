#include<mutex>
#include<atomic>

struct progress
{
    std::atomic<int> ByteCounter, copy_success_counter, copy_fail_counter;

    progress();
};


void track_progress(progress& progress_obj);