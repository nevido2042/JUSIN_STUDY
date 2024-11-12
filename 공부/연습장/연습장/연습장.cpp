#include "pch.h"

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <utility>

using namespace std;

// 구조체 정의
struct Node {
    int x, y;
    float g, h;
    Node* parent;

    Node(int x, int y, float g, float h, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), parent(parent) {}

    float f() const { return g + h; }

    bool operator<(const Node& other) const {
        return f() > other.f();  // 최소 힙을 위해 부등호를 반전
    }
};

// 휴리스틱 함수 (맨해튼 거리)
float heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// A* 알고리즘
void a_star(vector<vector<char>>& grid, pair<int, int> start, pair<int, int> end) {
    int rows = grid.size(), cols = grid[0].size();
    priority_queue<Node> open_list;
    unordered_map<int, Node*> all_nodes;
    open_list.emplace(start.first, start.second, 0.0, heuristic(start.first, start.second, end.first, end.second));

    vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };  // 상하좌우 이동

    while (!open_list.empty()) {
        Node current = open_list.top();
        open_list.pop();

        // 목표에 도달한 경우
        if (current.x == end.first && current.y == end.second) {
            while (current.parent) {
                grid[current.x][current.y] = '*';  // 경로 표시
                current = *current.parent;
            }
            break;
        }

        for (auto& dir : directions) {
            int new_x = current.x + dir.first;
            int new_y = current.y + dir.second;

            // 유효한 위치 체크
            if (new_x < 0 || new_x >= rows || new_y < 0 || new_y >= cols || grid[new_x][new_y] == '#')
                continue;

            float g_new = current.g + 1.0;
            float h_new = heuristic(new_x, new_y, end.first, end.second);

            Node* successor = new Node(new_x, new_y, g_new, h_new, new Node(current));
            open_list.push(*successor);
            all_nodes[new_x * cols + new_y] = successor;
        }
    }

    // 결과 출력
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            cout << cell << ' ';
        }
        cout << endl;
    }

    // 메모리 정리
    for (auto& [_, node] : all_nodes) {
        delete node;
    }
}

int main() {
    vector<vector<char>> grid = {
        {'S', '.', '.', '#', '.', '.', '.'},
        {'.', '#', '.', '#', '.', '#', '.'},
        {'.', '#', '.', '.', '.', '#', '.'},
        {'.', '.', '#', '#', '.', '.', '.'},
        {'#', '.', '#', 'E', '.', '#', '.'}
    };

    pair<int, int> start = { 0, 0 };
    pair<int, int> end = { 4, 3 };

    a_star(grid, start, end);

    return 0;
}
