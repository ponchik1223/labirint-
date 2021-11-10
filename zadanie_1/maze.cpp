#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;

const int wall = 0, pass = 1;



bool deadend(int x, int y, int** maze, int height, int width) {
    int a = 0;

    if (x != 1) {
        if (maze[y][x - 2] == pass)
            a += 1;
    }
    else a += 1;

    if (y != 1) {
        if (maze[y - 2][x] == pass)
            a += 1;
    }
    else a += 1;

    if (x != width - 2) {
        if (maze[y][x + 2] == pass)
            a += 1;
    }
    else a += 1;

    if (y != height - 2) {
        if (maze[y + 2][x] == pass)
            a += 1;
    }
    else a += 1;

    if (a == 4)
        return 1;
    else
        return 0;
}
void visual(int** maze, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            switch (maze[i][j]) {
            case wall: cout << "0 "; break;
            case pass: cout << "  "; break;
            }
        cout << endl;
    }
    cout << endl;
}

bool ended(int** maze, int height, int width) {
    bool b = 1;

    for (int i = 1; i < (height - 1); i += 2)
        for (int j = 1; j < (width - 1); j += 2)
            if (maze[i][j] == wall)
                b = 0;
    return b;
}

void mazemake(int** maze, int height, int width) {
    int x, y, c, a;
    bool b;

    for (int i = 0; i < height; i++) // Массив заполняется землей-ноликами
        for (int j = 0; j < width; j++)
            maze[i][j] = wall;

    x = 3; y = 3; a = 0; // Точка приземления крота и счетчик
    while (1) {
        a++;
        if (a % 100 == 0)
            if (ended(maze, height, width))
                break;
        maze[y][x] = pass;
        while (1) { // Бесконечный цикл, который прерывается только тупиком
            c = rand() % 4; // Напоминаю, что крот прорывает
            switch (c) {  // по две клетки в одном направлении за прыжок
            case 0: if (y != 1)
                if (maze[y - 2][x] == wall) { // Вверх
                    maze[y - 1][x] = pass;
                    maze[y - 2][x] = pass;
                    y -= 2;
                }
            case 1: if (y != height - 2)
                if (maze[y + 2][x] == wall) { // Вниз
                    maze[y + 1][x] = pass;
                    maze[y + 2][x] = pass;
                    y += 2;
                }
            case 2: if (x != 1)
                if (maze[y][x - 2] == wall) { // Налево
                    maze[y][x - 1] = pass;
                    maze[y][x - 2] = pass;
                    x -= 2;
                }
            case 3: if (x != width - 2)
                if (maze[y][x + 2] == wall) { // Направо
                    maze[y][x + 1] = pass;
                    maze[y][x + 2] = pass;
                    x += 2;
                }
            }
            if (deadend(x, y, maze, height, width))
                break;
        }

        if (deadend(x, y, maze, height, width)) // Вытаскиваем крота из тупика
            do {
                x = 2 * (rand() % ((width - 1) / 2)) + 1;
                y = 2 * (rand() % ((height - 1) / 2)) + 1;
            } while (maze[y][x] != pass);
    } // На этом и все.
}
