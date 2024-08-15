#include "online.h"
#include "ui_online.h"
#include <QDebug>
#include "tcpclient.h"
#include <QMessageBox>

Online::Online(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Online)
{
    ui->setupUi(this);
}

Online::~Online()
{
    delete ui;
}

void Online::showUsr(PDU *pdu)
{
    if(NULL == pdu)
    {
        return;
    }
    uint uiSize = pdu->uiMsgLen/32;
    char caTmp[32];
    for(int i=0;i<uiSize;i++)
    {
        memcpy(caTmp,(char*)(pdu->caMsg)+i*32,32);
        ui->online_lw->addItem(caTmp);
    }
}

void Online::on_add_friend_pb_clicked()
{
    QListWidgetItem *pItem = ui->online_lw->currentItem();
    if(NULL == pItem)
    {
        QMessageBox::warning(this, "添加好友", "请选择要添加的好友");
        return;
    }
    QString strPerUserName = pItem->text();
    QString strLoginName = TcpClient::getInstance().loginName();
    PDU *pdu = mkPDU(0);
    qDebug() << "on_addfriend_pb_clicked  " << strPerUserName << " " << strLoginName;
    pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    memcpy(pdu->caData, strPerUserName.toStdString().c_str(), strPerUserName.size());
    memcpy(pdu->caData + 32, strLoginName.toStdString().c_str(), strLoginName.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = NULL;

}
