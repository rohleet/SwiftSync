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

    for(const fs::directory_entry& entry : fs::directory_iterator(source)){
        cout<<entry.path();
    }
    
    return 0;
}