#include "tcpserver.h"
#include "ui_tcpserver.h"

#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include <QFile>
TcpServer::TcpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    loadConfig();

    MyTcpServer::getInstance().listen(QHostAddress(m_strIp),m_usPort);
}

TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::loadConfig()
{
    QFile file(":/server.config");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray baDate = file.readAll();
        QString strDate = baDate.toStdString().c_str();
        strDate.replace("\r\n"," ");
        QStringList strList = strDate.split(" ");
        m_strIp = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        qDebug()<<"ip:"<<m_strIp<<"port"<<m_usPort;
        file.close();
    }else {
        QMessageBox::critical(this,"open config","open corrte");
    }
}
