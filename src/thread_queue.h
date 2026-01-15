#include<queue>
#include<filesystem>

struct file_copy {
    std::filesystem::path source;
    std::filesystem::path destination;
    std::filesystem::copy_options c;

    file_copy(std::filesystem::path source,std::filesystem::path destination,std::filesystem::copy_options c);
};

class thread_queue
{
private:
    std::queue<file_copy> file_queue;

public:

    thread_queue();
    ~thread_queue();
    
    void push_to_queue(file_copy f);
    file_copy pop_from_queue();

    bool is_empty();
};