#include <vector>
#include <iostream>

using namespace std;

class Graph {
public:
    Graph(int count): V(count) {
        E.resize(count);
    }

    bool add_edge(int s, int e) {
        if (s < V) {
            E[s].push_back(e);
            return true;
        }
        return false;
    }

    void topo_sort_with_loop_detection(vector<int> targets) {
        vector<bool> discovered(V, false);
        vector<bool> finished(V, false);

        std::function<void(int)> helper = nullptr;
        helper = [&](int n) {
            discovered[n] = true;
            if (!finished[n]) {
                for (const auto &v: E[n]) {
                    if (discovered[v]) {
                        cout << "Loop found! << endl";
                        break;
                    }
                    if (!finished[v])
                        helper(v);
                }
            }
            finished[n] = true;
            discovered[n] = false;
        };
    }

private:
    int V;
    vector<vector<int>> E;
};