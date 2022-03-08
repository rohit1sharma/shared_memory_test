#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QBuffer>

int var =10;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),sharedMemory("QSharedMemory_practice")
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setText("whats the string, you want");

    connect(ui->pushButton_dec, &QPushButton::clicked,
            this, &MainWindow::dec);
    connect(ui->pushButton_inc, &QPushButton::clicked,
            this, &MainWindow::inc);
    connect(ui->pushButton_connect, &QPushButton::clicked,
            this, &MainWindow::connect1);
    connect(ui->pushButton_connect2, &QPushButton::clicked,
            this, &MainWindow::connect2);



}

void MainWindow::inc(){
    var++;
    QString st = QString::number(var);
         ui->label->setText(st);


        // load into shared memory
        QBuffer buffer;
        buffer.open(QBuffer::ReadWrite);
        QDataStream out(&buffer);
        out << st;
        int size = buffer.size();
        qDebug()<<size;
        sharedMemory.lock();
        char *to = (char*)sharedMemory.data();
        const char *from = buffer.data().data();
        memcpy(to, from, qMin(sharedMemory.size(), size));
        sharedMemory.unlock();

        QDataStream in(&buffer);

        QString got;

        sharedMemory.lock();
        qDebug()<<sharedMemory.size();
        buffer.setData((char*)sharedMemory.constData(), sharedMemory.size());
        buffer.open(QBuffer::ReadOnly);
        in >> got;
        qDebug()<<got;
        sharedMemory.unlock();
        ui->label->setText(got);

}


void MainWindow::dec(){
    var--;
    QString st = QString::number(var);
         ui->label->setText(st);


        // load into shared memory
        QBuffer buffer;
        buffer.open(QBuffer::ReadWrite);
        QDataStream out(&buffer);
        out << st;
        int size = buffer.size();
        qDebug()<<size;

        sharedMemory.lock();
        char *to = (char*)sharedMemory.data();
        const char *from = buffer.data().data();
        memcpy(to, from, qMin(sharedMemory.size(), size));
        sharedMemory.unlock();

        QDataStream in(&buffer);

        QString got;

        sharedMemory.lock();
        qDebug()<<sharedMemory.size();
        buffer.setData((char*)sharedMemory.constData(), sharedMemory.size());
        buffer.open(QBuffer::ReadOnly);
        in >> got;
        qDebug()<<got;
        sharedMemory.unlock();
        ui->label->setText(got);

}

void MainWindow::connect1()
{
    if (sharedMemory.isAttached())
        detach();
 ui->label->setText("whats the string, you want");
QString st = QString::number(var);

     ui->label->setText(st);
qDebug()<<st;

    // load into shared memory
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out << st;
    int size = buffer.size();
    qDebug()<<size;

    if (!sharedMemory.create(size)) {
        ui->label->setText(tr("Unable to create shared memory segment."));
        return;
    }
    sharedMemory.lock();
    char *to = (char*)sharedMemory.data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(sharedMemory.size(), size));
    sharedMemory.unlock();
}


void MainWindow::connect2()
{
    if (!sharedMemory.attach()) {
        ui->label->setText(tr("Unable to attach to shared memory segment.\n"));
        return;
    }

    QBuffer buffer;
    QDataStream in(&buffer);

    QString got;

    sharedMemory.lock();
    qDebug()<<sharedMemory.size();
    buffer.setData((char*)sharedMemory.constData(), sharedMemory.size());
    buffer.open(QBuffer::ReadOnly);
    in >> got;
    qDebug()<<got;
    sharedMemory.unlock();

    sharedMemory.detach();
    ui->label->setText(got);
}
void MainWindow::detach()
{
    if (!sharedMemory.detach())
        ui->label->setText(tr("Unable to detach from shared memory."));
}

MainWindow::~MainWindow()
{
    delete ui;
}

