#include <list>
#include <vector>
#include <iostream>
#include <unordered_map>

using namespace std;

class LRUCache {
private:
        int capacity_;
        using ListElement = std::pair<int,int>;
        std::list<ListElement> m_list;
        std::unordered_map<int, std::list<ListElement>::iterator> m_cache;
        std::function<void(int, int)> updateCache = [this](int k, int v) {
            auto ele = std::make_pair(k, v);
            m_list.push_front(ele);
            m_cache[k] = m_list.begin();
        };
public:
        LRUCache(int size): capacity_(size) {}

        bool getValue(int key, int& value) {
            auto it = m_cache.find(key);
            if (it == m_cache.end()) {
                return false;
            }

            // cache hit - remove and push front
            int k = it->second->first, v = it->second->second;
            m_list.erase(it->second);
            updateCache(k, v);
            value = it->second->second;
            return true;
        }

        void setValue(int key, int value) {
            auto it = m_cache.find(key);
            // found existing element delete it from std::list
            if (it != m_cache.end()) {
                std::cout << "found existing element" << std::endl;
                m_list.erase(it->second);
            }
            // did not find existing element and at max capacity, delete least recently used
            else if (m_cache.size() == capacity_) {
                std::cout << "reached capacity" << std::endl;
                auto k = m_list.back().second;
                m_list.pop_back();
                m_cache.erase(k);
            }

            updateCache(key, value);
        }

        bool delValue(int key) {
            auto it = m_cache.find(key);
            // found existing element delete it from cache
            if (it == m_cache.end()) {
                return false;
            }
            m_list.erase(it->second);
            m_cache.erase(it);
            return true;
        }

        void display() {
            for (auto &it : m_list) {
                std::cout << it.first << " " << it.second << std::endl;
            }
        }
};

int search(std::vector<int> &a, int k) {
    for (unsigned lo = 0, up = a.size(); lo < up; ) {
        unsigned m = lo + ((up - lo) >> 1);

        if (k == a[m]) return m;
        if (k < a[m]) up = m;
        else lo = m + 1;
    }
    return -1;
}

typedef bool (*compare)(int k, int e);

bool lower(int k, int e) {
    // cout << endl << "lower" << endl;
    return k <= e;
}

bool upper(int k, int e) {
    // cout << endl << "upper" << endl;
    return k < e;
}

int find(std::vector<int> &a, int k, compare cmp) {
    unsigned lo = 0, up = a.size();
    while (lo < up) {
        unsigned m = lo + ((up - lo) >> 1);
        if (cmp(k, a[m])) up = m;
        else lo = m + 1;
    }
    // if (lo < 0) lo = 0;
    if (lo < a.size()) {
        if(cmp == lower && a[lo] < k)
            lo++;
        else if (cmp == upper && a[lo] <= k)
            lo++;
    }
    if (lo >= a.size()) lo = a.size() - 1;
    // cout << endl << lo <<  "..." << a[lo] << endl;
    return lo;
}

int search_bounds(std::vector<int> &a, int k, std::function<bool(int k, int e)> comp) {
    unsigned lo = 0, up = a.size();
    while(lo < up) {
        unsigned m = lo + ((up - lo) >> 1);
        if (comp(k, a[m])) up = m;
        else lo = m + 1;
    }
    return lo;
}

struct node {
    int &val;
    int b;
    node *left, *right;
    // node(int v=0) : left(nullptr), right(nullptr), val(v) {}
    node():left(nullptr), right(nullptr), val(b) {}
    node(int &vr) : left(nullptr), right(nullptr), val(vr) {}
};
static node* root = nullptr;

void build_partial_sum_tree(vector<int>& a) {
    std::function<node*(int l, int r)> helper = nullptr;
    helper = [&](int l, int r) -> node* {
        if (l == r) {
            return new node(std::ref(a[l]));
        }
        int m = (l + r) >> 1;
        node* root = new node();
        root->left = helper(l, m);
        root->right = helper(m + 1, r);
        if (root->left && root->right) {
            root->val = root->left->val + root->right->val;
        }
        return root;
    };

    root = helper(0, a.size());
    cout << root->val << endl;
}

int sum(vector<int>& a, int i, int j) {
    return a[i] + a[j];
}

void update(vector<int>& a, int i, int val) {
    int l = 0, r = a.size();
    std::function<int(int l, int r)> helper = nullptr;
    helper = [&](int l, int r) -> int {
        if (l == r) {
            if (i == l) {
                auto d = a[i] - val;
                a[i] = val;
                cout << "d=" << d << " i=" << i << endl;
                return d;
            }
            return 0;
        }

        int m = (l + r) >> 1;
        int ld = helper(l, m);
        int rd = helper(m + 1, r);
        root->val += (ld + rd);
        return ld ? ld : rd;
    };
    helper(0, a.size());
}

int main () {
    std::vector<int> srchvec { -4, -2, -1, 0, 2, 4, 6, 7, 8, 10, 14 };
    build_partial_sum_tree(srchvec);
    cout << "sum(2,4)=" << sum(srchvec, 2, 4) << endl;
    update(srchvec, 3, 100);
    cout << "sum(2,4)=" << sum(srchvec, 2, 4) << endl;

    // std::cout << "finding -2=" << search(srchvec, -2) << endl;
    // std::cout << "finding -4=" << search(srchvec, -4) << endl;
    // std::cout << "finding 6=" << search(srchvec, 6) << endl;
    // std::cout << "finding 66=" << search(srchvec, 66) << endl;

    // auto lower = [](int k, int e) { return k <= e; };
    // auto upper = [](int k, int e) { return k < e; };
    // std::cout << "finding -2=" << search_bounds(srchvec, -2, lower) << endl;
    // std::cout << "finding -2=" << search_bounds(srchvec, -2, upper) << endl;
    // std::cout << "finding -4=" << search_bounds(srchvec, -4, lower) << endl;
    // std::cout << "finding -4=" << search_bounds(srchvec, -4, upper) << endl;
    // std::cout << "finding 6=" << search_bounds(srchvec, 6, lower) << endl;
    // std::cout << "finding 6=" << search_bounds(srchvec, 6, upper) << endl;
    // std::cout << "finding 66=" << search_bounds(srchvec, 66, lower) << endl;
    // std::cout << "finding 66=" << search_bounds(srchvec, 66, upper) << endl;

    // std::cout << "finding -2=" << find(srchvec, -2, lower) << endl;
    // std::cout << "finding -2=" << find(srchvec, -2, upper) << endl;
    // std::cout << "finding -4=" << find(srchvec, -4, lower) << endl;
    // std::cout << "finding -4=" << find(srchvec, -4, upper) << endl;
    // std::cout << "finding -6=" << find(srchvec, -6, lower) << endl;
    // std::cout << "finding -6=" << find(srchvec, -6, upper) << endl;
    // std::cout << "finding 6="  << find(srchvec, 6, lower) << endl;
    // std::cout << "finding 6="  << find(srchvec, 6, upper) << endl;
    // std::cout << "finding 66=" << find(srchvec, 66, lower) << endl;
    // std::cout << "finding 66=" << find(srchvec, 66, upper) << endl;
    // std::cout << "finding 9=" << find(srchvec, 9, lower) << endl;
    // std::cout << "finding 9=" << find(srchvec, 9, upper) << endl;
    // std::cout << "finding -3=" << find(srchvec, -3, lower) << endl;
    // std::cout << "finding -3=" << find(srchvec, -3, upper) << endl;
    // for (int i = 0; i < srchvec.size(); i++)
    //     cout << "[" << i << "]" << srchvec[i] << " ";
    // cout << endl;

    // LRUCache cache(4);
    // int val;
    // std::cout << "getValue(0)=" << cache.getValue(0, val) << std::endl;
    // std::cout << "getValue(0)=" << val << std::endl;

    // std::cout << "setValue(0, 0)" << std::endl;
    // cache.setValue(0,0);
    // cache.setValue(1,1);
    // cache.setValue(2,2);
    // cache.setValue(1,-1);
    // cache.setValue(3,3);
    // std::cout << "display" << std::endl;
    // cache.display();
    // cache.setValue(1,-2);
    // cache.setValue(4,4);

    // std::cout << "display" << std::endl;
    // cache.display();

    // std::cout << "getValue(0)=" << cache.getValue(0, val) << std::endl;
    // std::cout << "getValue(0)=" << val << std::endl;

    // std::cout << "delValue(1)=" << cache.delValue(1) << std::endl;
    // cache.display();

    // cache.setValue(0,0);
    // std::cout << "display" << std::endl;
    // cache.display();
// #define C_TO_F(degC) (((1.0 * (degC)) * (9.0/5)) + 32)
//     cout << C_TO_F(0) << endl;
//     cout << C_TO_F(-2.99) << endl;

    return 0;
}
