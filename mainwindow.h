#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();

    void on_LoadImage_clicked();

private:
    Ui::MainWindow *ui;
    void start();
    void process(unsigned char *pImage);
};

#endif // MAINWINDOW_H
