#include<iostream>
#include<fstream>
#include<filesystem>
#include<thread>
#include<chrono>
#include<vector>
using namespace std;
using namespace std::chrono;

namespace fs = std::filesystem;

void copy_single_file(fs::path p1,fs::path target,fs::copy_options c);

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

    vector<thread> threads;

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
                    threads.emplace_back(copy_single_file,p1,target,fs::copy_options::overwrite_existing);
                }
                continue;
            }
            // thread t(copy_single_file,p1,target,fs::copy_options::none);
            threads.emplace_back(copy_single_file,p1,target,fs::copy_options::none);
        }

    }

    for (auto& t : threads){
        t.join();
    }

    auto stop_time = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop_time-start_time);

    cout<<duration.count()/100<<endl;
    
    return 0;
}

void copy_single_file(fs::path p1,fs::path target,fs::copy_options c) {
    fs::copy_file(p1,target,c);
}