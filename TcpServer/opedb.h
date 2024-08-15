#ifndef OPEDB_H
#define OPEDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>



class OpeDB : public QObject
{
    Q_OBJECT
public:
    explicit OpeDB(QObject *parent = nullptr);
    static OpeDB& getInstance();
    void init();
    ~OpeDB();
    bool handleRegist(const char *name,const char *pwd);
    bool handleLogin(const char *name,const char *pwd);
    void handleoffline(const char *name);
    QStringList handleALLOnline();
    int handleSearchUsr(const char *name);
    int handleAddFriend(const char *pername,const char *name);
    int getIdByUserName(const char *name);                // 根据用户名获取用户id
    void handleAddFriendAgree(const char *addedName, const char *sourceName); // 处理同意好友申请
    QStringList handleFlushFriend(const char *name);
    bool handleDelFriend(const char *name,const char *friendName);
signals:

public slots:
private:
    QSqlDatabase m_bd; //连接数据库
};

#endif // OPEDB_H
