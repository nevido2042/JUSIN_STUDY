#include "pch.h"

#include <iostream>
#include <conio.h> // _kbhit, _getch
#include <vector>
#include <thread>
#include <chrono>

const int WIDTH = 10;
const int HEIGHT = 20;
std::vector<std::vector<int>> board(HEIGHT, std::vector<int>(WIDTH, 0)); // 게임판
int currentPiece = 0; // 현재 블록
int pieceX = WIDTH / 2, pieceY = 0; // 블록의 위치
bool gameOver = false;

// 테트리스 블록 모양 정의 (단순한 2x2 블록)
std::vector<std::vector<int>> tetromino[1] = {
    {{1, 1},
     {1, 1}}  // 2x2 블록
};

// 화면 출력
void DrawBoard() {
    system("cls"); // 콘솔 화면 지우기 (Windows 환경에서만 사용 가능)

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (board[y][x] == 1) {
                std::cout << "X"; // 블록이 있는 곳은 X로 출력
            }
            else {
                std::cout << "."; // 빈 곳은 .으로 출력
            }
        }
        std::cout << std::endl;
    }
}

// 블록을 화면에 그리기
void DrawPiece() {
    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 2; ++x) {
            if (tetromino[currentPiece][y][x] == 1) {
                board[pieceY + y][pieceX + x] = 1;
            }
        }
    }
}

// 블록을 이동시키는 함수
bool MovePiece(int dx, int dy) {
    // 현재 블록을 지우고
    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 2; ++x) {
            if (tetromino[currentPiece][y][x] == 1) {
                board[pieceY + y][pieceX + x] = 0;
            }
        }
    }

    // 블록을 새로운 위치로 이동
    pieceX += dx;
    pieceY += dy;

    // 충돌 검사
    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 2; ++x) {
            if (tetromino[currentPiece][y][x] == 1) {
                if (pieceX + x < 0 || pieceX + x >= WIDTH || pieceY + y >= HEIGHT || board[pieceY + y][pieceX + x] == 1) {
                    pieceX -= dx;  // 이동 취소
                    pieceY -= dy;  // 이동 취소
                    return false;
                }
            }
        }
    }

    return true;
}

// 키 입력 처리
void HandleInput() {
    if (_kbhit()) {
        char key = _getch();
        if (key == 'a') { // 왼쪽 이동
            MovePiece(-1, 0);
        }
        if (key == 'd') { // 오른쪽 이동
            MovePiece(1, 0);
        }
        if (key == 's') { // 아래로 이동
            if (!MovePiece(0, 1)) {
                // 블록이 더 이상 아래로 내려갈 수 없다면 새로운 블록 생성
                pieceX = WIDTH / 2;
                pieceY = 0;
                DrawPiece();
            }
        }
    }
}

// 게임 루프
void GameLoop() {
    while (!gameOver) {
        HandleInput();
        DrawBoard();
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); // 300ms마다 블록이 내려간다
    }
}

int main() {
    std::cout << "Welcome to Tetris!" << std::endl;
    GameLoop();
    return 0;
}
