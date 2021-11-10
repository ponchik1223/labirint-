#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QRect>
#include <vector>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    const int wall = -1, pass = -2;
    int w = 0;
    int h = 0;
    double step = 0;
    double step2 = 0;
    bool isSearch = false;
    bool isReady = false;
    int* px;
    int* py;
    int** maze;
    int len;
    std::vector <std::vector<int>> copy;

    bool IsPass(int x, int y);
    void mazemake(int** maze, int h3, int w3);
    bool ended(int** maze, int h2, int w2);
    bool desperate_situation(int x, int y, int** maze, int h1, int w1);
    bool search(int ax, int ay, int bx, int by);\
    bool IsSearch(int x, int y);
private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // MAINWINDOW_H

