#include <iostream>
#include <math.h>

#define MAXIMUM_STEPS 200

int *loadBoardDimensions() {
    static int dim[2];
    std::cin >> dim[0];
    std::cin >> dim[1];
    return dim;
}

int * findStartingPosition(int **board, int width, int height)  {
    int startY = 0, startX = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (board[y][x] > board[startY][startX]) {
                startY = y;
                startX = x;
            }
        }
    }
    int * startingPosition = new int[2];
    startingPosition[0] = startX;
    startingPosition[1] = startY;
    return startingPosition;
}

void printSolution(int y, int x, int * steps, int stepsLimit) {
    // print first row of solution - starting coordinates
    std::cout << x << " " << y << std::endl;
    for (int i = 0; i < stepsLimit; i++) {
        if (steps[i] == -1) {
            std::cout << std::endl;
            break;
        }
        switch (steps[i]) {
            case 0:
                std::cout << "U";
                break;
            case 1:
                std::cout << "R";
                break;
            case 2:
                std::cout << "D";
                break;
            case 3:
                std::cout << "L";
                break;
        }
    }
}

int **loadBoard(int width, int height) {
    int **board = new int *[height];
    for (int y = 0; y < height; y++) {
        int row;
        std::cin >> row;
        board[y] = new int[width];
        for (int x = width - 1; x >= 0; x--) {
            board[y][x] = row % 10;
            row /= 10;
        }
    }
    return board;
}

bool isMoveUpCorrect(int **board, int width, int height, int y, int x, bool **visited) {
    return y > 0 && (x == width || board[y - 1][x] > 0) && (x == 0 || board[y - 1][x - 1] > 0) && !visited[y-1][x];
}

bool isMoveDownCorrect(int **board, int width, int height, int y, int x, bool **visited) {
    return y < height && (x == width || board[y][x] > 0) && (x == 0 || board[y][x - 1] > 0) && !visited[y+1][x];
}

bool isMoveLeftCorrect(int **board, int width, int height, int y, int x, bool **visited) {
    return x > 0 && (y == 0 || board[y - 1][x - 1] > 0) && (y == height || board[y][x - 1] > 0) && !visited[y][x-1];
}

bool isMoveRightCorrect(int **board, int width, int height, int y, int x, bool **visited) {
    return x < width && (y == 0 || board[y - 1][x] > 0) && (y == height || board[y][x] > 0) && !visited[y][x+1];
}

void updateBoardMoveRight(int **board, int y, int x, int height, int diff = 1) {
    if (y != 0) {
        board[y - 1][x] -= diff;
    }
    if (y != height) {
        board[y][x] -= diff;
    }
}

void updateBoardMoveLeft(int **board, int y, int x, int height, int diff = 1) {
    if (y != 0) {
        board[y - 1][x - 1] -= diff;
    }
    if (y != height) {
        board[y][x - 1] -= diff;
    }
}

void updateBoardMoveUp(int **board, int y, int x, int width, int diff = 1) {
    if (x != 0) {
        board[y - 1][x - 1] -= diff;
    }
    if (x != width) {
        board[y - 1][x] -= diff;
    }
}

void updateBoardMoveDown(int **board, int y, int x, int width, int diff = 1) {
    if (x != 0) {
        board[y][x - 1] -= diff;
    }
    if (x != width) {
        board[y][x] -= diff;
    }
}


bool findSteps(int **board, int width, int height, int startY, int startX, int y, int x,
               int currentStep, int *steps, bool **visited, bool firstStep = false) {
    if (!firstStep) {
        if (y == startY && x == startX) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (board[y][x] != 0) {
                        return false;
                    }
                }
            }
            steps[currentStep] = -1;
            return true;
        } else {
            visited[y][x] = true;
        }
    }
    if (isMoveUpCorrect(board, width, height, y, x, visited)) {
        updateBoardMoveUp(board, y, x, width);
        steps[currentStep] = 0;
        if (findSteps(board, width, height, startY, startX, y - 1, x, currentStep + 1, steps, visited)) return true;
        updateBoardMoveDown(board, y - 1, x, width, -1);
    }
    if (isMoveRightCorrect(board, width, height, y, x, visited)) {
        updateBoardMoveRight(board, y, x, height);
        steps[currentStep] = 1;
        if (findSteps(board, width, height, startY, startX, y, x + 1, currentStep + 1, steps, visited)) return true;
        updateBoardMoveLeft(board, y, x + 1, height, -1);
    }
    if (isMoveDownCorrect(board, width, height, y, x, visited)) {
        updateBoardMoveDown(board, y, x, width);
        steps[currentStep] = 2;
        if (findSteps(board, width, height, startY, startX, y + 1, x, currentStep + 1, steps, visited)) return true;
        updateBoardMoveUp(board, y + 1, x, width, -1);
    }
    if (isMoveLeftCorrect(board, width, height, y, x, visited)) {
        updateBoardMoveLeft(board, y, x, height);
        steps[currentStep] = 3;
        if (findSteps(board, width, height, startY, startX, y, x - 1, currentStep + 1, steps, visited)) return true;
        updateBoardMoveRight(board, y, x - 1, height, -1);
    }
    visited[y][x] = false;
    return false;
}

void findAndPrintSolution(int **board, int width, int height) {
    int* startingPosition = findStartingPosition(board, width, height);
    int startX = startingPosition[0];
    int startY = startingPosition[1];
    int *steps = new int[MAXIMUM_STEPS];
    bool ** visited = new bool* [height+1];
    for (int y=0; y < height+1; y++) {
        visited[y] = new bool[width+1];
    }
    bool secondVariant = false;
    if (board[startY][startX] == 3) {
        findSteps(board, width, height, startY, startX, startY, startX, 0, steps, visited, true);
    } else {
        secondVariant = !findSteps(board, width, height, startY, startX, startY, startX,0, steps, visited, true);
        if (secondVariant) {
            findSteps(board, width, height, startY + 1, startX+1, startY + 1, startX+1,0, steps, visited, true);
        }
    }

    if (secondVariant) {
        printSolution(startY + 1, startX + 1, steps, MAXIMUM_STEPS);
    } else {
        printSolution(startY, startX, steps, MAXIMUM_STEPS);
    }
}

int main() {
    int *boardDimensions = loadBoardDimensions();
    int **board = loadBoard(boardDimensions[0], boardDimensions[1]);
    findAndPrintSolution(board, boardDimensions[0], boardDimensions[1]);
}
