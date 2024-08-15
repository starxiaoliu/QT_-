#ifndef BOOK_H
#define BOOK_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>

#include "protocal.h"
class Book : public QWidget
{
    Q_OBJECT
public:
    explicit Book(QWidget *parent = nullptr);
    void updateFileList(const PDU *pdu);
    void clearEnterDir();
    QString enterDir();
    void setDownloadStatus(bool status);
    bool getDownloadStatus();
    QString getSaveFilePath();
    QString getShareFileName();

    qint64 m_iTotal = 0;    //总的文件大小
    qint64 m_iRecved = 0;   //已收到多少
signals:

public slots:
    void createDir();
    void flushFile();
    void delDir();
    void renameFile();
    void enterDir(const QModelIndex &index);
    void returnPre();
    void delRegFile();
    void uploadFile();
    void uploadFileDate();
    void downloadFile();
    void shareFile();

    void moveFile();
    void selectDestDir();


private:
    QListWidget *m_pBookListW;
    QPushButton *m_pReturnPB;
    QPushButton *m_pCreateDirPB;
    QPushButton *m_pDelDirPB;
    QPushButton *m_pRenamePB;
    QPushButton *m_pFlushFilePB;
    QPushButton *m_pUploadPB;
    QPushButton *m_pDownLoadPB;
    QPushButton *m_pDelFilePB;
    QPushButton *m_pShareFilePB;
    QPushButton *m_pSelectMoveToDirPB;

    QString m_strENterDir;
    QString m_strUploadFilePath;
    QString m_strMoveFileName;
    QString m_strMoveFilePath;
    QString m_strDestDirPath;

    QTimer *m_pTimer;

    QString m_strSaveFilePath;
    bool m_bDownload;

    QString m_strShareFileName;

};

#endif // BOOK_H
