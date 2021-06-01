#include <map>
#include <list>
#include <iostream>
#include <unordered_map>

using namespace std;

class LRU {
private:
    list<int> m_dq;
    int m_capacity;
    unordered_map<int, list<int>::iterator> m_cache;

public:
    LRU(int capacity) {
        m_capacity = capacity;
    }

    bool get(int value) {
        // unordered_map<int, list<int>::iterator>::iterator um_iter = m_cache.find(value);
        auto iter = m_cache.find(value);
        if (iter == m_cache.end()) {
            return false;
        }
        m_dq.erase(iter->second);
        m_dq.push_front(value);
        m_cache[value] = m_dq.begin();
        return true;
    }

    bool del(int value) {
        auto iter = m_cache.find(value);
        if (iter == m_cache.end()) {
            return false;
        }
        m_dq.erase(iter->second);
        m_cache.erase(value);
        return true;
    }

    bool set(int value) {
        auto iter = m_cache.find(value);
        if (iter == m_cache.end()) {
            if (m_capacity == m_cache.size()) {
                cout << "cache full ";
                m_cache.erase(m_dq.back());
                m_dq.pop_back();
            }
        } 
        else {
            m_dq.erase(iter->second);
        }
        m_dq.push_front(value);
        m_cache[value] = m_dq.begin();
        return true;
    }

    void display(void) {
        for (auto it=m_dq.begin(); it != m_dq.end(); it++) {
            cout << *it << "-";
        }
        cout << "|" << endl;
    }
};