#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <unordered_set>

using namespace std;

struct Cell final {
    Cell() : row(0), col(0), cost(0), distance_from_top_left(numeric_limits<int>::max()) {}
    Cell(const int row_, const int col_, const int cost_)
     : row(row_), col(col_), cost(cost_), distance_from_top_left(numeric_limits<int>::max()) {}

    int row;
    int col;
    int cost;
    int distance_from_top_left;
    vector<Cell*> neighbors;
};

void SolveA(vector<vector<int>> matrix) {
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());

    // Initialize graph
    unordered_map<string, Cell> undirected_graph;
    undirected_graph.reserve(rows * cols);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            string hash{ to_string(r) + "_" + to_string(c) };

            if (undirected_graph.find(hash) == undirected_graph.cend()) {
                undirected_graph[hash] = Cell{r, c, matrix[r][c]};
            }

            Cell& cell = undirected_graph.at(hash);

            // Populate neighbors
            const int row = cell.row;
            const int col = cell.col;

            if (row == 0) {// Only have south neighbor
                const string south_hash{ to_string(row + 1) + "_" + to_string(col) };

                if (undirected_graph.find(south_hash) == undirected_graph.cend()) {
                    undirected_graph[south_hash] = Cell{row + 1, col, matrix[row + 1][col]};
                }

                Cell& south_cell = undirected_graph.at(south_hash);
                cell.neighbors.push_back(&south_cell);
            }
            else if (row == rows - 1) {// Only have north neighbor
                const string north_hash{ to_string(row - 1) + "_" + to_string(col) };

                if (undirected_graph.find(north_hash) == undirected_graph.cend()) {
                    undirected_graph[north_hash] = Cell{row - 1, col, matrix[row - 1][col]};
                }

                Cell& north_cell = undirected_graph.at(north_hash);
                cell.neighbors.push_back(&north_cell);
            }
            else {// Have south and north neighbors
                const string south_hash{ to_string(row + 1) + "_" + to_string(col) };

                if (undirected_graph.find(south_hash) == undirected_graph.cend()) {
                    undirected_graph[south_hash] = Cell{row + 1, col, matrix[row + 1][col]};
                }

                Cell& south_cell = undirected_graph.at(south_hash);
                cell.neighbors.push_back(&south_cell);

                const string north_hash{ to_string(row - 1) + "_" + to_string(col) };

                if (undirected_graph.find(north_hash) == undirected_graph.cend()) {
                    undirected_graph[north_hash] = Cell{row - 1, col, matrix[row - 1][col]};
                }

                Cell& north_cell = undirected_graph.at(north_hash);
                cell.neighbors.push_back(&north_cell);
            }

            if (col == 0) {// Only have east neighbor
                const string east_hash{ to_string(row) + "_" + to_string(col + 1) };

                if (undirected_graph.find(east_hash) == undirected_graph.cend()) {
                    undirected_graph[east_hash] = Cell{row, col + 1, matrix[row][col + 1]};
                }

                Cell& east_cell = undirected_graph.at(east_hash);
                cell.neighbors.push_back(&east_cell);
            }
            else if (col == cols - 1) {// Only have west neighbor
                const string west_hash{ to_string(row) + "_" + to_string(col - 1) };

                if (undirected_graph.find(west_hash) == undirected_graph.cend()) {
                    undirected_graph[west_hash] = Cell{row, col - 1, matrix[row][col - 1]};
                }

                Cell& west_cell = undirected_graph.at(west_hash);
                cell.neighbors.push_back(&west_cell);
            }
            else {// Have east and west neighbors
                const string east_hash{ to_string(row) + "_" + to_string(col + 1) };

                if (undirected_graph.find(east_hash) == undirected_graph.cend()) {
                    undirected_graph[east_hash] = Cell{row, col + 1, matrix[row][col + 1]};
                }

                Cell& east_cell = undirected_graph.at(east_hash);
                cell.neighbors.push_back(&east_cell);

                const string west_hash{ to_string(row) + "_" + to_string(col - 1) };

                if (undirected_graph.find(west_hash) == undirected_graph.cend()) {
                    undirected_graph[west_hash] = Cell{row, col - 1, matrix[row][col - 1]};
                }

                Cell& west_cell = undirected_graph.at(west_hash);
                cell.neighbors.push_back(&west_cell);
            }
        }
    }

    unordered_set<string> visited_cells;

    const auto compare = [](const Cell& a, const Cell& b) {
        return a.distance_from_top_left > b.distance_from_top_left;
    };
    priority_queue<Cell, vector<Cell>, decltype(compare)> min_heap(compare);
    
    Cell& start = undirected_graph["0_0"];
    start.distance_from_top_left = 0;

    min_heap.push(start);

    while (!min_heap.empty()) {
        Cell curr = min_heap.top();
        min_heap.pop();

        string visited_hash{ to_string(curr.row) + "_" + to_string(curr.col) };
        if (visited_cells.find(visited_hash) != visited_cells.cend()) {
            continue;
        }
        visited_cells.insert(visited_hash);

        for (Cell* neighbor : curr.neighbors) {
            const int new_distance = curr.distance_from_top_left + neighbor->cost;
            if (new_distance < neighbor->distance_from_top_left) {
                neighbor->distance_from_top_left = new_distance;
            }

            // const int neighbor_row = neighbor->row;
            // const int neighbor_col = neighbor->col;
            // const string neighbor_hash = to_string(neighbor_row) + "_" + to_string(neighbor_col);
            // if (visited_cells.find(neighbor_hash) == visited_cells.cend()) {
            //     min_heap.push(neighbor);
            // }

            min_heap.push(*neighbor);
        }
    }

    cout << "Lowest total risk of any path from the top left to the bottom right: " 
         << undirected_graph[to_string(rows - 1) + "_" + to_string(cols - 1)].distance_from_top_left << endl;
}

void SolveB(vector<vector<int>> matrix) {

}

int main() {
    ifstream input_file("input.txt");

    vector<vector<int>> matrix;

    string line;
    while (getline(input_file, line)) {
        vector<int> row;
        row.reserve(line.length());

        for (const char& c : line) {
            row.push_back(c - '0');
        }

        matrix.emplace_back(move(row));
    } 

    SolveA(matrix);
    //SolveB(matrix);

    return 0;
}