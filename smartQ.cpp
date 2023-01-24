#include <chrono>
#include <queue>

using namespace std;

using TimeStamp = std::chrono::time_point<std::chrono::system_clock>;
struct entry{
    int value;
    TimeStamp ts;
    entry(int v) {
        value = v;
        ts = std::chrono::system_clock::now();
    }
};

class Compare {
public:
    bool operator()(entry &rhs, entry &lhs) {
        return lhs.ts > rhs.ts;
    }
};

class CircularQ{
private:
    priority_queue<entry, vector<entry>, Compare> buffer_;
    int size, readers;
public:
    CircularQ(int sz, int numReaders) {
        size = sz;
        readers = numReaders;
    }

    int readNext(int readerId) {
        (void)readerId;
        buffer_.top();
    }

    void writeNext(int value) {
        buffer_.emplace(entry(value));
        if (buffer_.size() > size) {
            buffer_.pop();
        }
    }
};