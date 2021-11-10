#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    w = ui->spinBox->value();
    h = ui->spinBox_2->value();
    step = 500.0 / w;
    step2 = 500.0 / h;
    isReady = true;
    int** maze = new int* [h];
    for (int i = 0; i < h; i++)
        maze[i] = new int[w];

    mazemake(maze, h, w);

    maze[0][1] = -2;   // ставим вход и выход в лабиринт
    maze[h-2][w-1] = -2;
    const int I = h;
    std::vector<std::vector<int> > _v(I, vector<int>(I));   // костыль
    for (int i = 0; i < I; i++)
        for (int j = 0; j < I; j++)
                _v[i][j] = maze[i][j];
    copy = _v;                                            //

    isReady = true;
    for (int i = 0; i < h; i++)
        delete[] maze[i];
    delete[] maze;
    isSearch = false;
    repaint();

}
void MainWindow::on_pushButton_2_clicked() //начать поиск выхода
{
   search(1,0,w-1,h-2);
   isSearch = true;
   repaint();

}

bool MainWindow::IsSearch(int x, int y)
{
    for (int i =0; i< w*h; ++i)
    {
        if (px[i]==x && py[i]== y)
        {
            return true;
        }
     }
    return false;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter;
    QBrush brush;
    QBrush brush2;
    brush.setColor(Qt::black);
    brush2.setColor(Qt::red);
    brush.setStyle(Qt::SolidPattern);
    brush2.setStyle(Qt::SolidPattern);
    painter.begin(this);

    for( int i = 0; i<w; ++i)  // отрисовка лабиринта и правильного пути с учетов отступов
        {
        for(int j = 0; j<h;++j)
            {
            if (IsPass(i,j))
            {
                painter.setBrush(brush);
                painter.drawRect(step2*j + 5, i*step + 20, step2, step);
            }
            if (isSearch && IsSearch(j,i))
            {
                painter.setBrush(brush2);
                painter.drawRect((step2*j+step2/4) + 5, (i*step + step/4) + 20, step2/2, step/2);
            }
        }
    }

    painter.end();
}

bool MainWindow::IsPass(int x, int y)
{
    if (isReady)
    {
        return !(copy[x][y] + 1);
    }
    else
    {
        return 0;
    }
}

bool MainWindow::desperate_situation(int x, int y, int** maze, int h1, int w1) {
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

    if (x != w1 - 2) {
        if (maze[y][x + 2] == pass)
            a += 1;
    }
    else a += 1;

    if (y != h1 - 2) {
        if (maze[y + 2][x] == pass)
            a += 1;
    }
    else
    {
        a += 1;
    }
    if (a == 4)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool MainWindow::ended(int** maze, int h2, int w2) {
    bool b = 1;

    for (int i = 1; i < (h2 - 1); i += 2)
        for (int j = 1; j < (w2 - 1); j += 2)
            if (maze[i][j] == wall)
                b = 0;
    return b;
}

void MainWindow::mazemake(int** maze, int h3, int w3) {
    int x, y, c, a;

    for (int i = 0; i < h3; i++) // Массив заполняется землей
        for (int j = 0; j < w3; j++)
            maze[i][j] = wall;

    x = 3; y = 3; a = 0; // Точка из которой начинается ход
    while (1) {
        a++;
        if (a % 100 == 0)
            if (ended(maze, h3, w3))
                break;
        maze[y][x] = pass;
        while (1) {
            c = rand() % 4;
            switch (c) {
            case 0: if (y != 1)
                if (maze[y - 2][x] == wall) { // Вверх
                    maze[y - 1][x] = pass;
                    maze[y - 2][x] = pass;
                    y -= 2;
                }
            case 1: if (y != h3 - 2)
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
            case 3: if (x != w - 2)
                if (maze[y][x + 2] == wall) { // Направо
                    maze[y][x + 1] = pass;
                    maze[y][x + 2] = pass;
                    x += 2;
                }
            }
            if (desperate_situation(x, y, maze, h3, w3))
                break;
        }

        if (desperate_situation(x, y, maze, h3, w3)) // если тупик, идем в рандомную точку
            do {
                x = 2 * (rand() % ((w3 - 1) / 2)) + 1;
                y = 2 * (rand() % ((h3 - 1) / 2)) + 1;
            } while (maze[y][x] != pass);

    }
}


bool MainWindow::search(int ax, int ay, int bx, int by)   // волновой поиск
 {
  px = new int [w * h];
  py = new int [w * h];
  int dx[4] = {1, 0, -1, 0};
  int dy[4] = {0, 1, 0, -1};
  int d, x, y, k;
  bool stop;

  if (copy[ay][ax] == wall || copy[by][bx] == wall) return false;  // стена?

  // распространение волны
  d = 0;
  copy[ay][ax] = 0;            // стартовая ячейка
  do {
    stop = true;               // предполагаем, что все свободные клетки уже помечены
    for ( y = 0; y < h; ++y )
      for ( x = 0; x < w; ++x )
        if ( copy[y][x] == d )
        {
          for ( k = 0; k < 4; ++k )
          {
             int iy=y + dy[k], ix = x + dx[k];
             if ( iy >= 0 && iy < h && ix >= 0 && ix < w &&
                  copy[iy][ix] == pass )
             {
                stop = false;              // найдены непомеченные клетки
                copy[iy][ix] = d + 1;      // распространяем волну
             }
          }
        }
    d++;
  } while ( !stop && copy[by][bx] == pass );

  if (copy[by][bx] == pass) return false;  // путь не найден

  // восстановление пути
  len = copy[by][bx];
  x = bx;
  y = by;
  d = len;
  while ( d > 0 )
  {
    px[d] = x;
    py[d] = y;                   // записываем (x, y) в путь
    d--;
    for (k = 0; k < 4; ++k)
    {
       int iy=y + dy[k], ix = x + dx[k];
       if ( iy >= 0 && iy < h && ix >= 0 && ix < w &&
            copy[iy][ix] == d)
      {
          x = x + dx[k];
          y = y + dy[k];           // переходим в ячейку, которая на 1 ближе к старту
          break;
      }
    }
  }
  px[0] = ax;
  py[0] = ay;
  return true;

}
