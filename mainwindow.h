#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QMetaType>
#include <QString>
#include <QTcpServer>
#include <cstring>
#include <string>
#include <QTcpSocket>
#include "filemanager.h"
#include "HttpGenerator.h" 
#include "HttpProcessor.h"
#include <iostream>

#define PORT 8080

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void newMessage(QString);
private slots:
    void newConnection();
    void appendToSocketList(QTcpSocket* socket);
    void readSocket();
    void discardSocket();
    void displayMessage(const QString& str);
    void on_pushButton_sendMessage_clicked();

private:
    Ui::MainWindow *ui;
    /* QString buffer; */
    char buffer[300000];
    HttpProcessor processor;

    void sendMessage(QTcpSocket* socket, const char* message, int n);
    void displayMessage(const char* str);
    QTcpServer* m_server;
    QList<QTcpSocket*> connection_list;
};

#endif // MAINWINDOW_H
