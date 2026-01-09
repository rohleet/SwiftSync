#include<iostream>
#include<fstream>
#include<filesystem>
using namespace std;

namespace fs = std::filesystem;

void traverseDirectory(fs::path source) {

}

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
            fs::create_directories(target);

        //     fs::path directory_name;  // / entry.path().filename()

        //     string dir_name = "";

        //     fs::path temp_path = p1;

        //     while(1){
                
        //     }

        //     fs::create_directory(destination / directory_name);

        }
    }

    for(const fs::directory_entry& entry : fs::recursive_directory_iterator(source)){
        const fs::path& p1 = entry.path();

        fs::path relative_path = fs::relative(p1, source);
        fs::path target = destination / relative_path;
        
        if(entry.is_regular_file()){
            fs::copy_file(p1,target);
        }

    }
    
    return 0;
}