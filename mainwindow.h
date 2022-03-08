#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//first one
#include <QMainWindow>
#include <QDialog>
#include <QSharedMemory>
#include<QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
     QSharedMemory sharedMemory;
//      int var =10;
    ~MainWindow();

private:
    Ui::MainWindow *ui;

      public slots:
                    void inc();
                    void dec();
                    void connect1();
                    void connect2();
private:
   void detach();
};
#endif // MAINWINDOW_H
