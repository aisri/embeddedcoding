#include <chrono>
#include <atomic>
#include <iostream>

using namespace std::chrono_literals;

struct node {
    uint8_t data1, data2;
    std::atomic<node*> link;
    std::chrono::system_clock::time_point ts;
    node(uint8_t d1, uint8_t d2): data1(d1), data2(d2), link(nullptr){
        ts = std::chrono::system_clock::now();
    }
};

std::atomic<node*> head { nullptr };

int put_values(uint8_t data1, uint8_t data2) {
    node* nn = new node(data1, data2);
    if (!nn)
        return -1;
    nn->link.store(head);
    head.store(nn);
    return 0;
}

int get_values(uint8_t *bytes, int len) {
    std::atomic<node*> it;
    it.store(head);
    int idx = 0;
    while (it.load() && len >= 2) {
        auto now = std::chrono::system_clock::now();
        if (now - it.load()->ts <= 5s) {
            // std::cout << it.load()->data1 << std::endl;
            bytes[idx++] = it.load()->data1;
            bytes[idx++] = it.load()->data2;
            len -= 2;
        }
        it.store(it.load()->link);
    }

    std::atomic<node*> list;
    list.store(it.load()->link);
    it.load()->link.store(nullptr);

    return idx ? idx : -1;
}