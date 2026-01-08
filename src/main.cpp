#include<iostream>
#include<fstream>
#include<filesystem>
using namespace std;

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: swiftsync <source> <destination>\n";
        return 1;
    }

    fs::path source = argv[1];
    fs::path destination = argv[2];

    cout<<argv[1]<<endl;
    cout<<argv[2]<<endl;
    
    return 0;
}