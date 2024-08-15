#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include "protocal.h"
#include "opedb.h"
#include <QDir>
#include <QFile>
#include <QTimer>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    QString getName();
    void copyDir(QString strSrcDir, QString strDestDir);
signals:
    void offline(MyTcpSocket *mysocket);
public slots:
    void recvMsg();
    void clientoffline();
    void sendFileToClient();
private:
    QString m_strName;

    QFile m_file;
    qint64 m_iTotal;
    qint64 m_iReceived;
    bool m_bUpload;

    QTimer *m_pTimer;
};

#endif // MYTCPSOCKET_H
