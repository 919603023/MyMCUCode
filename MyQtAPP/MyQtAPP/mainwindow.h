#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QList>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void RecvStringHandle(QString buf);

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpSocket;
    QString buf;
    bool LightButtonFlag;
};
#endif // MAINWINDOW_H
