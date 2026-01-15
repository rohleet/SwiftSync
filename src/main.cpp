#include<iostream>
#include<fstream>
#include<filesystem>
#include<thread>
#include<chrono>
#include<vector>
#include<mutex>
#include "thread_queue.h"
using namespace std;
using namespace std::chrono;

namespace fs = std::filesystem;

void copy_single_file(thread_queue& t_queue, mutex& queue_operation_semaphore,int& producer_status);

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

        //     fs::path directory_name;  // / entry.path().filename()

        //     string dir_name = "";

        //     fs::path temp_path = p1;

        //     while(1){
                
        //     }

        //     fs::create_directory(destination / directory_name);

        }
    }

    auto start_time =  high_resolution_clock::now();

    // vector<thread> threads;

    thread_queue t_queue;
    mutex queue_operation_semaphore;

    int producer_status = 0; //0 when it is not running 1 while currenlty running and -1 for completed.

    thread copy_queue(copy_single_file,std::ref(t_queue),std::ref(queue_operation_semaphore),std::ref(producer_status));

    for(const fs::directory_entry& entry : fs::recursive_directory_iterator(source)){
        producer_status = 1;
    
        const fs::path p1 = entry.path();

        fs::path relative_path = fs::relative(p1, source);
        fs::path target = destination / relative_path;


        if(entry.is_regular_file()){

            if(fs::directory_entry(target).is_regular_file()){
                cout<<"THe file " <<p1.filename()<<" already exists. Do you want to replace it? Y/N"<<endl;
                char temp;
                cin>>temp;
                if(temp=='Y' || temp=='y'){
                    // thread t(copy_single_file,p1,target,fs::copy_options::overwrite_existing);
                    // threads.emplace_back(copy_single_file,p1,target,fs::copy_options::overwrite_existing);

                    queue_operation_semaphore.lock();
                    t_queue.push_to_queue(file_copy(p1,target,fs::copy_options::overwrite_existing));
                    queue_operation_semaphore.unlock();
                }
                continue;
            }
            // thread t(copy_single_file,p1,target,fs::copy_options::none);
            // threads.emplace_back(copy_single_file,p1,target,fs::copy_options::none);
            t_queue.push_to_queue(file_copy(p1,target,fs::copy_options::none));
        }
    }

    producer_status = -1;

    copy_queue.join();

    // for (auto& t : threads){
    //     t.join();
    // }

    auto stop_time = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop_time-start_time);

    cout<<duration.count()/100<<endl;
    
    return 0;
}

void copy_single_file(thread_queue& t_queue,mutex& queue_operation_semaphore,int& producer_status) {

    while(1){

        if(producer_status==-1){
            break;
        }

        queue_operation_semaphore.lock();
        if(t_queue.is_empty()){
            queue_operation_semaphore.unlock();
            continue;
        }
        queue_operation_semaphore.unlock();
        

        queue_operation_semaphore.lock();
        file_copy file_detail = t_queue.pop_from_queue();
        queue_operation_semaphore.unlock();

        fs::copy_file(file_detail.source,file_detail.destination,file_detail.c);

    }
}