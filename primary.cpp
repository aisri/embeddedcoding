#include <iostream>

using namespace std;

#include "cache.cpp"
#include "lists.cpp"

void test_lru_cache(void) {
    LRU cache(3);

    cout << endl << "=-------=" << endl;
    cout << "LRU CACHE" << endl;
    cout << "=========" << endl;

    cout << "get(3) " << cache.get(3) << endl;
    cout << "set(3) " << cache.set(3) << endl;
    cout << "set(5) " << cache.set(5) << endl;
    cout << "set(6) " << cache.set(6) << endl;
    cache.display();
    cout << "set(3) " << cache.set(3) << endl;
    cache.display();
    cout << "get(5) " << cache.get(5) << endl;
    cache.display();
    cout << "get(7) " << cache.get(7) << endl;
    cout << "set(7) " << cache.set(7) << endl;
    cache.display();
    cout << "del(7) " << cache.del(3) << endl;
    cache.display();
    cache.get(3);
    cout << "set(12) " << cache.set(12) << endl;
    cache.display();
}


void test_sll(void) {
    sll lst;

    cout << endl << "=----------------=" << endl;
    cout << "SINGLE LINKED LIST" << endl;
    cout << "==================" << endl;
    cout << "insert(4) " << lst.insert(4)->data << endl;
    cout << "insert(5) " << lst.insert(5)->data << endl;
    cout << "insert(7) " << lst.insert(7)->data << endl;
    cout << "insert(9) " << lst.insert(9)->data << endl;
    lst.display();
    lst.recursive_reverse();
    lst.display();
    cout << "pop() " << lst.pop()->data << endl;
    cout << "pop() " << lst.pop()->data << endl;
    lst.display();
    cout << "reverse() " << lst.reverse() << endl;
    lst.display();
    cout << "insert(3) " << lst.insert(3)->data << endl;
    lst.display();
    cout << "remove(4) " << lst.remove(4) << endl;
    lst.display();
    cout << "remove(5) " << lst.remove(5) << endl;
    lst.display();
    cout << "remove(7) " << lst.remove(7) << endl;
    lst.display();

    sll lst1;
    cout << "insert(4) " << lst1.insert(4)->data << endl;
    cout << "insert(5) " << lst1.insert(5)->data << endl;
    cout << "insert(7) " << lst1.insert(7)->data << endl;
    cout << "insert(9) " << lst1.insert(9)->data << endl;
    lst1.display();

    cout << endl << "Added lists:" << endl;
    cout << "list A => ";
    lst.display();
    cout << "list B => ";
    lst1.display();
    auto sum = lst1.add(&lst);
    sum->reverse();
    cout << "list Sum => ";
    sum->display();
}


int main() {
    test_lru_cache();
    test_sll();
    return 0;
}