#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    /* Setting up a new QTcpServer */ 
    m_server = new QTcpServer();
    if(m_server->listen(QHostAddress::Any, PORT)) {
        connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
        this->ui->statusBar->showMessage("Server is listening...");
        connect(this->ui->lineEdit_message, SIGNAL(returnPressed()), this, SLOT(on_pushButton_sendMessage_clicked()));
    }
    else {
        QMessageBox::critical(this,"QTCPServer",QString("Unable to start the server: %1.").arg(m_server->errorString()));
        exit(EXIT_FAILURE);
    }
}

MainWindow::~MainWindow() {
    foreach (QTcpSocket* socket, connection_list) {
        socket->close();
        socket->deleteLater();
    }
    m_server->close();
    m_server->deleteLater();
    delete ui;
}

void MainWindow::newConnection() {
    while (m_server->hasPendingConnections()) {
        QTcpSocket* socket = m_server->nextPendingConnection();
        appendToSocketList(socket);
        QString tmp = QString("Client has connect with IP %1 as %2").arg(socket->peerAddress().toString()).arg(socket->socketDescriptor());
        displayMessage(tmp);
    }
}

void MainWindow::appendToSocketList(QTcpSocket* socket) {
    connection_list.append(socket);
    connect(socket, SIGNAL(readyRead()), this , SLOT(readSocket()));
    connect(socket, SIGNAL(disconnected()), this , SLOT(discardSocket()));
}

void MainWindow::readSocket() {
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    qint64 block_size = socket->read(buffer, 300000);
    string response;
    processor.process(buffer, (qint64) block_size, response);
    displayMessage(buffer);
    displayMessage("===============================================");
    /* displayMessage(QString::fromUtf8(response.c_str())); */
    /* displayMessage("==============================================="); */
    /* cout << response << '\n'; */
    if (response != "")
        sendMessage(socket, response.c_str(), response.size());
}

void MainWindow::discardSocket() {
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    for (int i=0;i<connection_list.size();i++) {
        if (connection_list.at(i) == socket) {
            connection_list.removeAt(i);
            break;
        }
    }
    socket->deleteLater();
}

void MainWindow::on_pushButton_sendMessage_clicked() {
    QString str = this->ui->lineEdit_message->text();
    str = str.trimmed();
    this->ui->lineEdit_message->clear();
    QString tmp = QString("Sent %1 to ").arg(str);

    this->ui->lineEdit_message->clear();
}

void MainWindow::sendMessage(QTcpSocket* socket, const char* message, int n) {
    if(socket) {
        if(socket->isOpen()) {
            socket->write(message, qint64(n));
        }
        else
            QMessageBox::critical(this,"QTCPServer","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"QTCPServer","Not connected");
}

void MainWindow::displayMessage(const QString& str) {
    this->ui->textBrowser_receivedMessages->append(str);
}

void MainWindow::displayMessage(const char* str) {
    this->ui->textBrowser_receivedMessages->append(str);
}
