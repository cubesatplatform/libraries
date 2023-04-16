#include "dfs.h"
#include <cmath>
#include <stack>
#include <consoleio.h>



using namespace std;

std::vector<std::vector<double>> initFrame(float *frame){      
    std::vector<std::vector<double>> v;
    int rows = 24;
    int cols = 32;


    int count=0;

    for (int i = 0; i < rows; ++i) {
        std::vector<double> row;
        for (int j = 0; j < cols; ++j) {
            row.push_back(frame[count]);
            count++;
        }
        v.push_back(row);
    }
    return v;
}







CDFS::CDFS()    {}

void CDFS::setGrid(const vector<vector<double>> &newGrid) {
    grid = newGrid;
    visited.assign(grid.size(), vector<bool>(grid[0].size(), false));
}

bool CDFS::isValid(int x, int y) {
    return x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size();
}

int CDFS::dfs(int x, int y, vector<pair<int, int>> &coordinates) {
    stack<pair<int, int>> stk;
    stk.push({x, y});
    visited[x][y] = true;
    int clusterSize = 0;

    while (!stk.empty()) {
        pair<int, int> current = stk.top();
        stk.pop();
        coordinates.push_back(current);
        clusterSize++;

        for (int dir = 0; dir < 4; ++dir) {
            int newX = current.first + dx[dir];
            int newY = current.second + dy[dir];

            if (isValid(newX, newY) &&
                !visited[newX][newY] &&
                abs(grid[current.first][current.second] - grid[newX][newY]) <= TEMP_THRESHOLD) {
                visited[newX][newY] = true;
                stk.push({newX, newY});
            }
        }
    }

    return clusterSize;
}

pair<double, double> CDFS::findPrimaryClusterCenter() {
    int maxClusterSize = 0;
    pair<double, double> center(-1, -1);

    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[0].size(); ++j) {
            if (!visited[i][j]) {
                vector<pair<int, int>> coordinates;
                int currentClusterSize = dfs(i, j, coordinates);

                if (currentClusterSize > maxClusterSize) {
                    maxClusterSize = currentClusterSize;

                    double sumX = 0;
                    double sumY = 0;
                    for (const auto &coord : coordinates) {
                        sumX += coord.first;
                        sumY += coord.second;
                    }

                    center = {sumX / currentClusterSize, sumY / currentClusterSize};
                }
            }
        }
    }

    return center;
}
