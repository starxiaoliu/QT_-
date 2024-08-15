#include "book.h"
#include "tcpclient.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include "opewidget.h"
#include "sharefile.h"

Book::Book(QWidget *parent) : QWidget(parent)
{
    m_strENterDir.clear();

    m_bDownload = false;
    m_pTimer = new QTimer;

    m_pBookListW = new QListWidget;
    m_pReturnPB = new QPushButton("返回");
    m_pCreateDirPB = new QPushButton("创建文件夹");
    m_pDelDirPB = new QPushButton("删除文件夹");
    m_pRenamePB = new QPushButton("重命名文件夹");
    m_pFlushFilePB = new QPushButton("刷新文件");

    QVBoxLayout *pDirVBL = new QVBoxLayout;
    pDirVBL->addWidget(m_pReturnPB);
    pDirVBL->addWidget(m_pCreateDirPB);
    pDirVBL->addWidget(m_pDelDirPB);
    pDirVBL->addWidget(m_pRenamePB);
    pDirVBL->addWidget(m_pFlushFilePB);

    m_pUploadPB = new QPushButton("上传文件");
    m_pDownLoadPB = new QPushButton("下载文件");
    m_pDelFilePB = new QPushButton("删除文件");
    m_pShareFilePB = new QPushButton("共享文件");

    QVBoxLayout *pFileVBL = new QVBoxLayout;
    pFileVBL->addWidget(m_pUploadPB);
    pFileVBL->addWidget(m_pDownLoadPB);
    pFileVBL->addWidget(m_pDelFilePB);
    pFileVBL->addWidget(m_pShareFilePB);


    QHBoxLayout *pMain = new QHBoxLayout;
    pMain->addWidget(m_pBookListW);
    pMain->addLayout(pDirVBL);
    pMain->addLayout(pFileVBL);

    setLayout(pMain);

    connect(m_pCreateDirPB,SIGNAL(clicked(bool))
            ,this,SLOT(createDir()));
    connect(m_pFlushFilePB,SIGNAL(clicked(bool))
            ,this,SLOT(flushFile()));
    connect(m_pDelDirPB,SIGNAL(clicked(bool))
            ,this,SLOT(delDir()));
    connect(m_pRenamePB,SIGNAL(clicked(bool)),
            this,SLOT(renameFile()));
    connect(m_pBookListW,SIGNAL(doubleClicked(QModelIndex))
            ,this,SLOT(enterDir(QModelIndex)));
    connect(m_pReturnPB, SIGNAL(clicked(bool))
            , this, SLOT(returnPre()));
    connect(m_pUploadPB, SIGNAL(clicked(bool))
            , this, SLOT(uploadFile()));
    connect(m_pTimer, SIGNAL(timeout())
            , this, SLOT(uploadFileDate()));
    connect(m_pDelFilePB, SIGNAL(clicked(bool))
            , this, SLOT(delRegFile()));
    connect(m_pDownLoadPB, SIGNAL(clicked(bool))
            , this, SLOT(downloadFile()));
    connect(m_pShareFilePB, SIGNAL(clicked(bool))
            , this, SLOT(shareFile()));
    //connect(m_pMoveFilePB, SIGNAL(clicked(bool))
            //, this, SLOT(moveFile()));
    //connect(m_pSelectMoveToDirPB, SIGNAL(clicked(bool))
            //, this, SLOT(selectDestDir()));

}

void Book::updateFileList(const PDU *pdu)
{

    if(NULL == pdu)
    {
        return;
    }
    QListWidgetItem *pItemTmp = NULL;
    int row = m_pBookListW->count();
    while(m_pBookListW->count()>0)
    {
        pItemTmp = m_pBookListW->item(row-1);
        m_pBookListW->removeItemWidget(pItemTmp);
        delete pItemTmp;
        row = row-1;
    }
    FileInfo *pFileInfo = NULL;
    int iCount = pdu->uiMsgLen/sizeof(FileInfo);
    for(int i=0;i<iCount;i++)
    {
        pFileInfo = (FileInfo*)(pdu->caMsg)+i;
        qDebug() << pFileInfo->caFileName << pFileInfo->iFileType;
        QListWidgetItem *pItem = new QListWidgetItem;
        if(0==pFileInfo->iFileType)
        {
          pItem->setIcon(QIcon(QPixmap(":/2.png")));
        }
        else if(1==pFileInfo->iFileType)
        {
            pItem->setIcon(QIcon(QPixmap(":/1.jpg")));
        }
        pItem->setText(pFileInfo->caFileName);
        m_pBookListW->addItem(pItem);
    }
}

void Book::clearEnterDir()
{
    m_strENterDir.clear();
}

QString Book::enterDir()
{
    return m_strENterDir;
}

void Book::setDownloadStatus(bool status)
{
    m_bDownload = status;
}

bool Book::getDownloadStatus()
{
    return m_bDownload;
}

QString Book::getSaveFilePath()
{
    return m_strSaveFilePath;
}

QString Book::getShareFileName()
{
    return m_strShareFileName;
}

void Book::createDir()
{
    QString strNewDir = QInputDialog::getText(this,"新建文件夹","新文件名字");
    if(!strNewDir.isEmpty())
    {
        if(strNewDir.size()>32)
        {
            QMessageBox::warning(this,"新建文件夹","新文件夹名字不能超过32个字符！");

        }
        else
        {
            QString strName = TcpClient::getInstance().loginName();
            QString strCurPath = TcpClient::getInstance().curPath();
            PDU *pdu = mkPDU(strCurPath.size()+1);
            pdu->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_REQUEST;
            strncpy(pdu->caData,strName.toStdString().c_str(),strName.size());
            strncpy(pdu->caData+32,strNewDir.toStdString().c_str(),strNewDir.size());
            memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
            TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
            free(pdu);
            pdu = NULL;
        }

    }
    else
    {
        QMessageBox::warning(this,"新建文件夹","新文件不能为空");
    }

}

void Book::flushFile()
{
    clearEnterDir();
    QString strCurPath = TcpClient::getInstance().curPath();
    PDU *pdu = mkPDU(strCurPath.size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    strncpy((char*)(pdu->caMsg),strCurPath.toStdString().c_str(),strCurPath.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Book::delDir()
{
    QString strCurPath = TcpClient::getInstance().curPath();
    QListWidgetItem *pItem = m_pBookListW->currentItem();
    if(NULL==pItem)
    {
        QMessageBox::warning(this,"删除文件","请选择要删除的文件");
    }
    else
    {
        QString strDelName = pItem->text();
        PDU *pdu = mkPDU(strCurPath.size()+1);
        pdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_REQUEST;
        strncpy((char*)(pdu->caData),strDelName.toStdString().c_str(),strDelName.size());
        memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
}

void Book::renameFile()
{
    QString strCurPath = TcpClient::getInstance().curPath();
    QListWidgetItem *pItem = m_pBookListW->currentItem();
    if(NULL==pItem)
    {
        QMessageBox::warning(this,"重命名文件","请选择要重命名的文件");
    }
    else
    {
        QString strOldName = pItem->text();
        QString strNewName = QInputDialog::getText(this,"重命名文件","请输入新的文件名");
        if(!strNewName.isEmpty())
        {
            PDU *pdu = mkPDU(strCurPath.size()+1);
            pdu->uiMsgType = ENUM_MSG_TYPE_RENAME_FILE_REQUEST;
            strncpy(pdu->caData,strOldName.toStdString().c_str(),strOldName.size());
            strncpy(pdu->caData+32,strNewName.toStdString().c_str(),strNewName.size());
            memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());

            TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
            free(pdu);
            pdu = NULL;
        }
        else
        {
            QMessageBox::warning(this,"重命名文件","新文件名称不为空");
        }
    }
}

void Book::enterDir(const QModelIndex &index)
{
    QString selectDirName = index.data().toString();    //选择的文件名字
    m_strENterDir = selectDirName;
    QString strCurPath = TcpClient::getInstance().curPath();
    PDU *pdu = mkPDU(strCurPath.size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_ENTER_DIR_REQUEST;
    strncpy(pdu->caData, selectDirName.toStdString().c_str(), selectDirName.size());
    memcpy(pdu->caMsg, strCurPath.toStdString().c_str(), strCurPath.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Book::returnPre()
{
    QString strCurPath = TcpClient::getInstance().curPath();
        QString strRootPath = "./" + TcpClient::getInstance().loginName();
        if(strCurPath == strRootPath)
        {
            QMessageBox::warning(this, "返回上一级目录", "已经在根目录");
        }
        else
        {
          //"目录的格式： ./aa/bb/cc"
            int index = strCurPath.lastIndexOf('/');
            QString newPath = strCurPath.remove(index, strCurPath.size() - index);
            qDebug() << "上一级目录为：" << newPath;
            TcpClient::getInstance().setCurPath(newPath);
            clearEnterDir();
            flushFile();
        }
}

void Book::delRegFile()
{
    QString strCurPath = TcpClient::getInstance().curPath();
    QListWidgetItem *pItem = m_pBookListW->currentItem();
    if(NULL == pItem)
    {
        QMessageBox::warning(this, "删除文件", "请选择删除的文件");
    }
    else
    {
        QString deleteName = pItem->text();
        PDU *pdu = mkPDU(strCurPath.size() + 1);
        pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FILE_REQUEST;
        strncpy((char*)(pdu->caData), deleteName.toStdString().c_str(), deleteName.size());
        memcpy(pdu->caMsg, strCurPath.toStdString().c_str(), strCurPath.size());
        TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
}

void Book::uploadFile()
{
    QString strCurPath = TcpClient::getInstance().curPath();
    m_strUploadFilePath = QFileDialog::getOpenFileName();
    qDebug() << m_strUploadFilePath;
    if(NULL == m_strUploadFilePath)
    {
        QMessageBox::warning(this, "上传文件", "文件不能为空");
    }
    else
    {

        int index = m_strUploadFilePath.lastIndexOf('/');
        QString newFileName = m_strUploadFilePath.right(m_strUploadFilePath.size() - index - 1);
        qDebug() << newFileName;

        QFile file(m_strUploadFilePath );
        qint64 uploadFileSize = file.size();
        QString strCurPath = TcpClient::getInstance().curPath();
        PDU *pdu = mkPDU(strCurPath.size() + 1);
        pdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
        memcpy(pdu->caMsg, strCurPath.toStdString().c_str(), strCurPath.size());
        sprintf(pdu->caData, "%s %lld", newFileName.toStdString().c_str(), uploadFileSize);
        TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = NULL;

        m_pTimer->start(1000);
    }

}

void Book::uploadFileDate()
{
    m_pTimer->stop();
        QFile file(m_strUploadFilePath);
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this, "上传文件", "打开文件失败");
            return;
        }
        char *pBuffer = new char[4096];
        qint64 ret = 0;
        while(true)
        {
            ret = file.read(pBuffer, 4096);
            if(ret > 0 && ret <= 4096)
            {
                TcpClient::getInstance().getTcpSocket().write(pBuffer, ret);
            }
            else if(0 == ret)
            {
                break;
            }
            else
            {
                QMessageBox::warning(this, "上传文件", "上传文件失败");
                break;
            }
        }
        file.close();
        delete []pBuffer;
        pBuffer = NULL;
}

void Book::downloadFile()
{
    QListWidgetItem *pItem = m_pBookListW->currentItem();
    if(NULL == pItem)
    {
        QMessageBox::warning(this, "下载文件", "请选择下载的文件");
    }
    else
    {
        QString strFileSavePath = QFileDialog::getSaveFileName();
        if(strFileSavePath.isEmpty())
        {
            QMessageBox::warning(this, "下载文件", "请选择文件保存位置");
            m_strSaveFilePath.clear();
        }
        else
        {
            m_strSaveFilePath = strFileSavePath;

        }
        QString strCurPath = TcpClient::getInstance().curPath();
        QString downloadName = pItem->text();
        PDU *pdu = mkPDU(strCurPath.size() + 1);
        pdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
        strcpy(pdu->caData, downloadName.toStdString().c_str());
        memcpy(pdu->caMsg, strCurPath.toStdString().c_str(), strCurPath.size());
        TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
}

void Book::shareFile()
{
    QListWidgetItem *pItem = m_pBookListW->currentItem();
    if(NULL == pItem)
    {
        QMessageBox::warning(this, "分享文件", "请选择分享的文件");
        return;
    }
    else
    {
        m_strShareFileName = pItem->text();
        qDebug() << "选中的文件为： " << m_strShareFileName;
    }
    Friend *pFriend = OpeWidget::getInstance().getFriend();
    QListWidget *pFriendList = pFriend->getFriendList();
    ShareFile::getInstance().updateFriend(pFriendList);
    if(ShareFile::getInstance().isHidden())
    {
        ShareFile::getInstance().show();
    }

}

void Book::moveFile()
{
    QListWidgetItem *pItem = m_pBookListW->currentItem();
    if(NULL == pItem)
    {
        QMessageBox::warning(this, "移动文件", "请选择移动的文件");
    }
    else
    {
        m_strMoveFileName = pItem->text();
        qDebug() << "移动的文件为： " << m_strMoveFileName;
        QString strCurPath = TcpClient::getInstance().curPath();
        m_strMoveFilePath = strCurPath + '/' + m_strMoveFileName;
        m_pSelectMoveToDirPB->setEnabled(true);

    }
}
void Book::selectDestDir()
{
    QListWidgetItem *pItem = m_pBookListW->currentItem();
    if(NULL == pItem)
    {
        QMessageBox::warning(this, "移动文件", "请选择移动到的文件夹");
    }
    else
    {
        QString destDirName = pItem->text();
        QString strCurPath = TcpClient::getInstance().curPath();
        m_strDestDirPath = strCurPath + '/' + destDirName;

        int srcLen = m_strMoveFilePath.size();
        int destLen = m_strDestDirPath.size();
        PDU *pdu = mkPDU(srcLen + destLen + 2);
        pdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_REQUEST;
        sprintf(pdu->caData, "%d %d %s", srcLen, destLen, m_strMoveFileName.toStdString().c_str());
        memcpy(pdu->caMsg, m_strMoveFilePath.toStdString().c_str(), srcLen);
        memcpy((char*)(pdu->caMsg) + (srcLen + 1), m_strDestDirPath.toStdString().c_str(), destLen);
        TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    m_pSelectMoveToDirPB->setEnabled(false);
}
