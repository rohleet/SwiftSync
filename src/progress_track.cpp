#include "progress_track.h"

using namespace std;

void progress::track_progress() {

    while(1) {

        unique_lock<mutex> lock(progress_mutex);

        p_cv.wait(lock, [&] {
            return workers_empty() || close_load();
        });

        if(close){
            return;
        }

        cout<<"Bytes Copied : "<<byte_counter<<endl<<"Number of succesfully copies files : "<<copy_success_counter<<endl<<"Number of attempts failed : "<<copy_fail_counter<<endl;
    }
}

progress::progress() {
    byte_counter = 0;
    copy_success_counter=0;
    copy_fail_counter=0;
}

void progress::close_progress_tracker() {
    close = true;
    p_cv.notify_all();
}

void progress::increment_byte_counter(uintmax_t increment) {
    byte_counter+=increment;
}

void progress::increment_success_counter() {
    copy_success_counter++;
}

void progress::increment_failed_counter() {
    copy_fail_counter++;
}

void progress::incrment_worker_buffer(){
    workers_number_buffer++;
}

void progress::decrement_worker_buffer() {
    workers_number_buffer--;
    p_cv.notify_one();
}

bool progress::workers_empty() {
    if(workers_number_buffer==0){
        return true;
    }        
    return false;
}

bool progress::close_load() {
    return close;
}