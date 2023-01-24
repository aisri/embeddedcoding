#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <condition_variable>
#include <chrono>

using namespace std::chrono_literals;
using namespace std;

/*
Single Linked List
int data type

Routine that accepts two params
p1 - head
p2 - integer
search for integer and if found remove the node
*/

class SLL {
    struct node {
        int data;
        node* link;
        node(int v, node *l=nullptr): data(v), link(l){}
    };
private:
    struct node* head;

public:
    SLL(): head(nullptr) {}

    bool removeNode(int val) {
        // val = 4
        // head -> 4 -> |
        auto it = head;
        auto prev = head;
        while (it) {
            if (it->data == val) {
                break;
            }
            prev = it; // 4
            it = it->link; // nullptr
        }
        if (it) {
            if (it == head) {
                head = it->link;
            } else {
                prev->link = it->link;
            }
            delete it;
            return true;
        }
        return false;
    }

    bool detectLoop() {
        // slow and fast pointer
    }
};

// static and dynamic polymorphism
// abstract class
// SIMD
/*
single producer w/ multiple consumers
multiple producer w/ multiple consumers
*/


class CircularQ {
private:
    atomic<int> front_;
    atomic<int> rear_;
    vector<int> buffer_;
    int size_;

public:
    CircularQ(int capacity): size_(capacity) {
        front_ = rear_ = -1;
        buffer_.resize(capacity);
    }

    bool empty() {
        return front_ == -1;
    }

    bool full() {
        return (rear_ + 1) % size_ == front_;
    }

    int get() {

    }

    bool set(int val) {
        if (full())
            return false;
    }
};

int main()
{
    CircularQ Q(1024);

    auto producer = [&]() {
        unique_lock<mutex> lk;
        condition_variable cv;
        auto predicate = [&] {
            return !Q.full();
        };

        while (true) {
            cv.wait_for(lk, 10ms, predicate);
            auto r = rand();
            Q.set(r);
        }
    };

    auto consumer = [&]() {
        while (true) {
            cout << Q.get() << endl;
        }
    };

    int p_count = 5;
    while (p_count--) {
        std::thread(producer);
    }

    int c_count = 4;
    while (c_count--) {
        std::thread(consumer);
    }
}

/**
 * store words of a dictionary
 * autofill feature first two or 3 letters of word
 * Trie - prefix tree
 *  space < O(n) * 42
 *   26^42
 * 0 - 26
 * 1 - 26, 26, 26, 26, 26
 * 2 - 26, 26, 26, 26,
 * time - linear inseration
 * vector<char>
 * map<char, trienode*>
 */