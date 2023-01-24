#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

using SensorMap = unordered_map<string, int>;

class Delivery {
public:
    Delivery(string dest, unsigned dist)
        : destination_(dest)
        , distance_(dist)
    {
    }
    SensorMap getNeededSensors() const
    {
        SensorMap map;
        if (distance_ < 10) {
            map["gps"] = 1;
            map["temp"] = 1;
            map["mass"] = 0;
            count_ = 2;
        } else if (distance_ >= 10 && distance_ < 100) {
            map["gps"] = 1;
            map["temp"] = 2;
            map["mass"] = 1;
            count_ = 4;
        } else if (distance_ >= 100) {
            map["gps"] = 2;
            map["temp"] = 4;
            map["mass"] = 2;
            count_ = 8;
        }
        return map;
    }
    string destination_;
    unsigned distance_;
    mutable unsigned count_;
};

class Scheduler {
public:
    Scheduler(SensorMap& conf)
        : available_sensors_(conf)
    {
    }

    bool can_be_delivered(SensorMap& m)
    {
        return m["gps"] < available_sensors_["gps"]
            && m["temp"] < available_sensors_["temp"]
            && m["mass"] < available_sensors_["mass"];
    };
    vector<Delivery> getSchedule(const vector<Delivery>& deliveries)
    {
        vector<Delivery> sched;

        for (const auto& d : deliveries) {
            auto m = d.getNeededSensors();
            if (can_be_delivered(m)) {
                available_sensors_["gps"] -= m["gps"];
                available_sensors_["temp"] -= m["temp"];
                available_sensors_["mass"] -= m["mass"];
                sched.emplace_back(d);
            }
        }
        return sched;
    }

    vector<Delivery> getBestSchedule(const vector<Delivery>& deliveries)
    {
        vector<Delivery> sched;
        struct DeliveryCompare {
            bool operator()(Delivery& d1, Delivery& d2) {
                return d1.count_ > d2.count_;
            }
        };

        priority_queue<Delivery, vector<Delivery>, DeliveryCompare> q;
        for (const auto& d : deliveries) {
            q.push(d);
        }

        while (!q.empty()) {
            auto d = q.top();
            q.pop();
            cout << "testing for -> " << d.destination_ << endl;
            auto m = d.getNeededSensors();
            if (can_be_delivered(m)) {
                available_sensors_["gps"] -= m["gps"];
                available_sensors_["temp"] -= m["temp"];
                available_sensors_["mass"] -= m["mass"];
                sched.emplace_back(d);
            }
        }
        return sched;
    }
    SensorMap& available_sensors_;
};

int main() {
    auto aa = Delivery("A", 0);
    auto bb = Delivery("B", 50);
    auto cc = Delivery("C", 200);

    SensorMap scheduler_resources { { "gps", 2 }, { "mass", 4 }, { "temp", 10 } };
    auto sched = Scheduler(scheduler_resources);
    for (auto &d: sched.getSchedule({ aa, bb, cc })) {
        cout << d.destination_ << " ";
    }
    cout << endl << "Done! Greedy Approach" << endl;

    SensorMap scheduler_resources1 { { "gps", 2 }, { "mass", 4 }, { "temp", 10 } };
    auto sched1 = Scheduler(scheduler_resources1);
    for (auto &d: sched1.getBestSchedule({ aa, bb, cc })) {
        cout << d.destination_ << " ";
    }
    cout << endl << "Done! Best Schedule" << endl;

    return 0;
}