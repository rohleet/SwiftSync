#include "thread_queue.h"

using namespace std;

file_copy::file_copy(std::filesystem::path source,std::filesystem::path destination,std::filesystem::copy_options c){
    this->source=source;
    this->destination=destination;
    this->c=c;
}

void thread_queue::push_to_queue(file_copy f){
    file_queue.push(f);
}

file_copy thread_queue::pop_from_queue(){
    auto val = file_queue.front();
    file_queue.pop();
    return val;
}

thread_queue::thread_queue()
{
}

thread_queue::~thread_queue()
{
}

bool thread_queue::is_empty(){
    return file_queue.empty();
}

