#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>//通信套接字
#include<QHostAddress>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_L->setText("光照传感器异常");
    ui->label_D->setText("异常");



    ui->label->setText("建立连接失败");
    tcpSocket = new QTcpSocket(this);
    ui->pushButton_L->setText("点击开灯");
    ui->label_L_Pic->setPixmap(QPixmap(":/light_down.png"));
    tcpSocket->connectToHost(QHostAddress("129.211.89.48"),8000);
    connect(tcpSocket, &QTcpSocket::connected,[=](){
        ui->label->setText("成功和服务器建立连接");
    }
    );
    connect(tcpSocket,&QTcpSocket::readyRead,[=](){
        //获取对方发送的内容
        buf = tcpSocket->readAll();
        //显示在编辑区
        qDebug()<< buf;
        RecvStringHandle(buf);
    }
    );
    connect(ui->pushButton_Flush,&QPushButton::clicked,[=](){
        tcpSocket->write("flush");
    });
    connect(ui->pushButton_L,&QPushButton::clicked,[=](){
        if(LightButtonFlag == false)
        {
            tcpSocket->write("open");
            LightButtonFlag = true;
            ui->pushButton_L->setText("点击关灯");
            ui->label_L_Pic->setPixmap(QPixmap(":/light_on.png"));
        }
        else
        {
            tcpSocket->write("close");
            LightButtonFlag = false;
            ui->pushButton_L->setText("点击开灯");
            ui->label_L_Pic->setPixmap(QPixmap(":/light_down.png"));
        }

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RecvStringHandle(QString buf)
{
    if(buf.indexOf(":") >= 0)
    {

        QList<QString> lst;

        lst=buf.split(':',QString::SkipEmptyParts);//lst={"one","two","three","four","five"}
        ui->label_T->setText(lst[0]);
        ui->label_H->setText(lst[1]);
        if(lst[2] == "N")
        {
            ui->label_L->setText("光照环境较好");
        }
        else if(lst[2] == "D")
        {
            ui->label_L->setText("光照环境较暗");
        }
        else if(lst[2] == "")
        {
            ui->label_L->setText("光照环境较亮");
        }
        else
        {
            ui->label_L->setText("光照传感器异常");
        }
        if(lst[3] == "N")
        {
            ui->label_D->setText("正常");
        }
        else
        {
            ui->label_D->setText("异常");
        }
        if(lst[4] == "open")
        {
            LightButtonFlag = true;
            ui->pushButton_L->setText("点击关灯");
            ui->label_L_Pic->setPixmap(QPixmap(":/light_on.png"));
        }
        else if(lst[4] == "close")
        {
            LightButtonFlag = false;
            ui->pushButton_L->setText("点击开灯");
            ui->label_L_Pic->setPixmap(QPixmap(":/light_down.png"));
        }
    }
}

