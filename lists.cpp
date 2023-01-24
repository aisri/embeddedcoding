#include <iostream>
#include <algorithm>

using namespace std;

struct node {
    int data;
    struct node *next;
    node(int value, struct node *link=nullptr) : data(value), next(link){}
};

class sll
{
public:
    struct node *head = nullptr;

public:
    sll() {
    }
    struct node *insert(int value) {
        auto v = new node(value);
        if (v == nullptr)
            return nullptr;

        v->next = head;
        head = v;
        return head;
    }

    struct node *pop(void) {
        if (!head)
            return nullptr;
        auto it = head;
        head = head->next;
        return it;
    }

    bool remove(int value) {
        if (!head)
            return false;

        auto it = head;
        // delete head if it matches
        if (head->data == value) {
            head = head->next;
            delete it;
            return true;
        }

        auto prev = head;
        for (auto it = head->next; it; it=it->next) {
            if (it->data == value) {
                prev->next = it->next;
                delete it;
                return true;
            }
            prev = it;
        }
        return false;
    }

    int len(void) {
        int count = 0;
        for (auto it = head; it; it = it->next, count++);
        return count;
    }

    bool reverse() {
        // if length = 0 or 1 return
        if (!head)
            return false;

        auto rear = (node*)0;
        auto cur = head, front = head->next;

        while (cur) {
            cur->next = rear;
            rear = cur;
            cur = front;
            if (front) front = front->next;
        }

        head = rear;
        return true;
    }

    struct node *helper(struct node *root) {
        if (!root->next) {
            head = root;
            return root;
        }
        auto ds_node = helper(root->next);
        ds_node->next = root;
        return root;
    }

    void recursive_reverse(void) {
        auto ds_node = helper(head);
        ds_node->next = nullptr;
    }

    sll* add(sll *right) {
        struct node *a = this->head;
        struct node *b = right->head;

        int asz = this->len();
        int bsz = right->len();

        int hsz = min(asz, bsz);

        sll *sum = new sll;
        int carry = 0;
        while (hsz) {
            sum->insert((a->data + b->data + carry) % 10);
            carry = (a->data + b->data + carry) / 10;

            a = a->next;
            b = b->next;
            hsz--;
        }

        auto tail = a ? a : b;
        while (tail) {
            sum->insert((carry + tail->data) % 10);
            carry = (carry + tail->data) / 10;
            tail = tail->next;
        }

        return sum;
    }

    void display(void) {
        auto it = head;
        while (it) {
            cout << it->data << "->";
            it = it->next;
        }
        cout << "|" << endl;
    }

    bool copy(struct node *src) {
        for(auto it = src; it; it=it->next)
            insert(it->data);
        return true;
    }

    ~sll() {
        auto it = head;
        while (it) {
            auto todo = it;
            it = it->next;
            delete todo;
        }
    }
};
