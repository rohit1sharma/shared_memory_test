#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QBuffer>
#include <QTimer>
int var =10;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),sharedMemory("QSharedMemory_practice")
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QString st1 = QString::number(var);
    ui->label->setText("whats the string you want");

    connect(ui->pushButton_dec, &QPushButton::clicked,
            this, &MainWindow::dec);
    connect(ui->pushButton_inc, &QPushButton::clicked,
            this, &MainWindow::inc);
    connect(ui->pushButton_connect, &QPushButton::clicked,
            this, &MainWindow::connect1);
    connect(ui->pushButton_connect2, &QPushButton::clicked,
            this, &MainWindow::connect2);


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(connect2()));
    timer->start(1);

}

//void MainWindow::change(QString got){
//    qDebug()<< "**************";
//      ui->label->setText(got);
//       qDebug()<< "11111";
//      QLabel *label1 = new QLabel(this);
//       qDebug()<< "*222222222";
//      label1->setFrameStyle(QFrame::Panel | QFrame::Sunken);
//       qDebug()<< "**333333333";
//      label1->setText(got);
//       qDebug()<< "*444444444444";
//      label1->setAlignment(Qt::AlignBottom | Qt::AlignRight);
//       qDebug()<< "*55555555555555";
////      label1->setGeometry(QRectF(10,10,30,80));
//}
//void MainWindow::trigg(){

//    QBuffer buffer;
//    QDataStream in(&buffer);

//    QString got;


//   if( sharedMemory.lock())
//   {
//       qDebug()<<"got the lock";
//    buffer.setData((char*)sharedMemory.constData(), sharedMemory.size());
//    buffer.open(QBuffer::ReadOnly);
//    in >> got;
//  if(sharedMemory.unlock()){
//    qDebug()<<"unlocked";
//  qDebug()<<got;
//  }

//}
////    sharedMemory.detach();
//    ui->label->setText(got);
////   change(got);

//}

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

//   while(1)
//    {
//        QThread::msleep(1000);
//        MainWindow::trigg();
//    }
//        while(1)
//           QTimer::singleShot(200, this, &MainWindow::trigg);
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

