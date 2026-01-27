#include<queue>
#include<filesystem>
#include<mutex>
#include<atomic>
#include <condition_variable>




struct file_copy {

    std::filesystem::path source;
    std::filesystem::path destination;
    std::filesystem::copy_options c;

    file_copy(std::filesystem::path source,std::filesystem::path destination,std::filesystem::copy_options c);
    file_copy();
    
};

class thread_queue
{
private:

    std::queue<file_copy> file_queue;
    std::mutex queue_operation_semaphore;
    std::condition_variable cv;
    bool close = 0; //0 when it is running 1 after the end of directory travel.

public:

    thread_queue();
    ~thread_queue();
    
    void push_to_queue(file_copy f);
    bool pop_from_queue(file_copy& out);

    void shutdown();

};
