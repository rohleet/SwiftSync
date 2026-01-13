#include<queue>
#include<filesystem>

using namespace std;

struct file_copy {
    std::filesystem::path& source;
    std::filesystem::path& destination;
    std::filesystem::copy_options c;
};

class thread_queue
{
private:
    queue<file_copy> file_queue;

public:

    thread_queue();
    ~thread_queue();
    
    void push_to_queue(file_copy f);
    file_copy pop_from_queue();
};

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
