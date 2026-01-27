#include<iostream>
#include<fstream>
#include<filesystem>
#include<thread>
#include<chrono>
#include<vector>
#include<mutex>
#include<atomic>
#include "thread_queue.h"
#include "progress_track.h"

using namespace std;
using namespace std::chrono;

namespace fs = std::filesystem;

void copy_single_file(thread_queue& t_queue);

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cerr << "Usage: swiftsync <source> <destination>\n";
        return 1;
    }

    fs::path source = argv[1];
    fs::path destination = argv[2];

    if (!fs::exists(source) || !fs::is_directory(source)) {
        if(!fs::exists(source)){
            cerr<<"Source directory does not exist";
        } else {
            cerr<<"The given path is not a directory";
        }
        return 1;
    }

    destination /= source.filename();

    fs::create_directory(destination);

    
    for(const fs::directory_entry& entry : fs::recursive_directory_iterator(source)){
        const fs::path& p1 = entry.path();

        if(entry.is_directory()){

            fs::path relative_path = fs::relative(p1, source);
            fs::path target = destination / relative_path;

            if(fs::directory_entry(target).is_directory()){
                continue;
            }

            fs::create_directories(target);

        }
    }

    thread_queue t_queue;

    unsigned int workers = thread::hardware_concurrency();
    vector<thread> pool;

    progress progress_obj;

    thread progress_tracker_thread(track_progress,progress_obj);

    for(unsigned int i = 0; i<workers; i++){
        pool.emplace_back(copy_single_file,std::ref(t_queue));
    }

    auto start_time =  high_resolution_clock::now();


    for(const fs::directory_entry& entry : fs::recursive_directory_iterator(source)){
    
        const fs::path p1 = entry.path();

        fs::path relative_path = fs::relative(p1, source);
        fs::path target = destination / relative_path;


        if(entry.is_regular_file()){

            if(fs::directory_entry(target).is_regular_file()){
                cout<<"THe file " <<p1.filename()<<" already exists. Do you want to replace it? Y/N(Defualt:N)"<<endl;
                char temp;
                cin>>temp;
                if(temp=='Y' || temp=='y'){

                    t_queue.push_to_queue(file_copy(p1,target,fs::copy_options::overwrite_existing));

                }
                continue;
            }
            t_queue.push_to_queue(file_copy(p1,target,fs::copy_options::none));
        }
    }

    t_queue.shutdown();

    for(thread& t : pool){
        t.join();
    }

    progress_tracker_thread.join();

    // for (auto& t : threads){
    //     t.join();
    // }

    auto stop_time = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop_time-start_time);

    cout<<duration.count()/100<<endl;
    
    return 0;
}

void copy_single_file(thread_queue& t_queue) {

    while(true) {

        file_copy file_detail;
        if(!t_queue.pop_from_queue(file_detail)){
            return;
        }

        fs::copy_file(file_detail.source,file_detail.destination,file_detail.c);
    }
    
}
