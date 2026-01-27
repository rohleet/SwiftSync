#include "progress_track.h"

using namespace std;

void progress::track_progress() {

    mutex pgress;
    condition_variable cv;

    while(1) {

        unique_lock<mutex> lock(pgress);

        cv.wait(lock, [&] {
            return true;
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