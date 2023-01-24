#include "dp.h"

#include <iostream>


int findPaths(vector<vector<int>> grid, int r, int c, unordered_map<pair<int,int>> memo={}) {
    if (r > grid.size() || c > grid[0].size()) {
        return 0;
    }

    if (r == grid.size() - 1 && c == grid[r].size() - 1) {
        return 1;
    }

    return findPaths(grid, r+1, c) + findPaths(grid, r, c+1);
}
