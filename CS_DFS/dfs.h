#ifndef CDFS_H
#define CDFS_H

#include <vector>
#include <utility>

//DFS  Depth First Search Clustering Algorithm

std::vector<std::vector<double>> initFrame(float *frame);

class CDFS {
private:
    const int dx[4]={-1, 0, 1, 0};
    const int dy[4]={0, 1, 0, -1};

     
    const double TEMP_THRESHOLD=1.0;
    std::vector<std::vector<double>> grid;
    std::vector<std::vector<bool>> visited;

    bool isValid(int x, int y);
    int dfs(int x, int y, std::vector<std::pair<int, int>> &coordinates);

public:
    CDFS();
    void setGrid(const std::vector<std::vector<double>> &newGrid);
    std::pair<double, double> findPrimaryClusterCenter();
};

#endif // CDFS_H
