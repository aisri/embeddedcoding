#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using namespace std;

typedef void (*callback)();

void set_hw_callback(uint64_t schedule_time, callback cb);

struct SoftwareTimer {
    uint64_t expiration;
    callback task;
    SoftwareTimer(uint64_t e, callback t)
        : expiration(e)
        , task(t)
    {
    }
};

struct SWTimerDiff {
    bool operator()(SoftwareTimer& t1, SoftwareTimer& t2)
    {
        return t2.expiration > t1.expiration;
    }
};

// min heap to store timers
priority_queue<SoftwareTimer, vector<SoftwareTimer>, SWTimerDiff> timers;
static SoftwareTimer current(0, nullptr);
std::mutex access_timers;

void handle_hw_callback()
{
    std::scoped_lock<mutex> lock { access_timers };
    auto tmr = timers.top();
    timers.pop();
    tmr.task();

    if (!timers.empty()) {
        set_hw_callback(timers.top().expiration, handle_hw_callback);
        current = timers.top();
    }
}

void schedule_sw_timer(uint64_t abs_time, callback cb)
{
    std::scoped_lock<mutex> lock { access_timers };
    timers.push(SoftwareTimer(abs_time, cb));

    auto tmr = timers.top();
    if (current.expiration != tmr.expiration) {
        set_hw_callback(tmr.expiration, handle_hw_callback);
        if (current.expiration)
            timers.push(current);
        current = tmr;
    }
}
