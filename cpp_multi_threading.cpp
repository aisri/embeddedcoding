#include <deque>
#include <iostream>
#include <future>
#include <chrono>

using namespace std;
using namespace std::literals;

struct result {
    unsigned long long value;
    unsigned N;
};

result factorial(std::shared_future<unsigned> N)
{
    unsigned long long prod = 1;
    unsigned n = N.get();
    for (int i = 2; i <= n; i++) {
        prod *= i;
    }
    result res { prod, n };
    return res;
}

class A{
public:
    void f(int x, char c){}
    long g(double x) { return 0; }
    int operator()(int N) {
        return N * 2;
    };
};

void foo(int X) { }

std::deque<packaged_task<int()>> task_q;
std::mutex mu;
std::condition_variable cv;

void pt_exec()
{
    std::packaged_task<int()> pt;
    {
        std::unique_lock<mutex> lock(mu);
        cv.wait(lock, [] {return !task_q.empty()});
        pt = std::move(task_q.front());
        task_q.pop_front();
    }
    pt();
}

int main()
{
    std::thread th(pt_exec);
    std::packaged_task<int()> pt(bind(factorial, 5));
    {
        std::unique_lock<mutex> lock(mu);
        task_q.push_back(pt);
    }
    cv.notify_one();
    th.join();
    cout << pt.get_future().get() << endl;

    // A a;
    // std::thread t1(a, 6);
    // std::thread t2(std::ref(a), 6);
    // std::thread t3(A(), 6);
    // std::thread t4([](int x) { return x * x; });
    // std::thread t5(foo, 7);

    // std::thread t6(&A::f, a, 8, 'w');
    // std::thread t7(&A::g, &a, 9.99);
    // std::thread t8(std::move(a), 6);
    // std::bind, std::call_once, std::async

    // std::promise<unsigned> p;
    // std::future<unsigned> pf = p.get_future();
    // std::shared_future<unsigned> spf = pf.share();

    // std::future<result> fut = std::async(std::launch::async, factorial, spf);
    // std::future<result> fut1 = std::async(std::launch::async, factorial, spf);
    // std::future<result> fut2 = std::async(std::launch::async, factorial, spf);

    // std::this_thread::sleep_for(100ms);
    // p.set_value(7);

    // result res = fut.get();
    // cout << res.N << "! = " << res.value << endl;
    // res = fut1.get();
    // cout << res.N << "! = " << res.value << endl;
    // res = fut2.get();
    // cout << res.N << "! = " << res.value << endl;
    return 0;
}
