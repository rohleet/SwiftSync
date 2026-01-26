#include<iostream>
#include<fstream>
#include<filesystem>
#include<thread>
#include<chrono>
#include<vector>
#include<mutex>
#include<atomic>
#include "thread_queue.h"
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

    auto start_time =  high_resolution_clock::now();

    // vector<thread> threads;

    thread_queue t_queue;

    thread copy_queue1(copy_single_file,std::ref(t_queue));
    thread copy_queue2(copy_single_file,std::ref(t_queue));
    thread copy_queue3(copy_single_file,std::ref(t_queue));
    thread copy_queue4(copy_single_file,std::ref(t_queue));
    thread copy_queue5(copy_single_file,std::ref(t_queue));


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
                    // thread t(copy_single_file,p1,target,fs::copy_options::overwrite_existing);
                    // threads.emplace_back(copy_single_file,p1,target,fs::copy_options::overwrite_existing);

                    t_queue.push_to_queue(file_copy(p1,target,fs::copy_options::overwrite_existing));
                }
                continue;
            }
            // thread t(copy_single_file,p1,target,fs::copy_options::none);
            // threads.emplace_back(copy_single_file,p1,target,fs::copy_options::none);
            t_queue.push_to_queue(file_copy(p1,target,fs::copy_options::none));
        }
    }

    t_queue.shutdown();


    copy_queue1.join();
    copy_queue2.join();
    copy_queue3.join();
    copy_queue4.join();
    copy_queue5.join();

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

        // if(popped){
        //     if(producer_end_status){
        //         break;
        //     } else {
        //         continue;
        //     }
        // } 

        fs::copy_file(file_detail.source,file_detail.destination,file_detail.c);
    }
    
}