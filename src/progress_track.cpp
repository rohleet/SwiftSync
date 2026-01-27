#include "progress_track.h"

using namespace std;

void track_progress(progress& progress_obj) {

    mutex pgress;
    condition_variable cv;

    while(1) {

        unique_lock<mutex> lock(pgress);

        cv.wait(lock, [] {
            return false || true;
        })

        
    }
}

progress::progress() {
    ByteCounter = 0;
    copy_success_counter=0;
    copy_fail_counter=0;
}