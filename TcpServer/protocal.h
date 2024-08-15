#ifndef PROTOCAL_H
#define PROTOCAL_H
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
typedef unsigned int uint;

#define REGIST_OK "regist ok"
#define REGIST_FAILED "resist failed : name existed"

#define LOGIN_OK "login ok"
#define LOGIN_FAILED "login failed : name or pwd error or relogin"
#define SEARCH_USER_NO "no such people"
#define SEARCH_USER_ONLINE "online"
#define SEARCH_USER_OFFLINE "offline"

#define UNKNOW_ERROR "unknow_error"
#define EXISTED_FRIEND "friend exist"
#define ADD_FRIEND_OFFLINE "user offline"
#define ADD_FRIEND_NOEXIST "user not exist"
#define ADD_FRIEND_OK "add friend ok"

#define DEL_FRIEND_OK "delete friend ok"
#define CREATE_DIR_OK "create dir ok"
#define DIR_NO_EXIST "cur dir not exist"
#define FILE_NAME_EXIST "file name exist"

#define DEL_DIR_OK "delete dir ok"
#define DEL_DIR_FAILURED "delete dir failured: is regualer file"

#define RENAME_FILE_OK "rename file ok"
#define RENAME_FILE_FAILURED "rename file failured"

#define ENTER_DIR_FAILED "enter dir failed"

#define UPLOAD_FILE_OK "upload file ok"
#define UPLOAD_FILE_FAILED "upload file failed"

#define FILE_DELETE_OK "file delete ok"
#define FILE_DELETE_FAILED "file delete failed"

#define SHARE_FILE_OK "share file ok"
#define SHARE_FILE_FAILED "share file failed"

#define MOVE_FILE_OK "move file ok"
#define MOVE_FILE_FAILED "move file failed, not dir"

#define COMMON_ERROR "failed:system is busy"

enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN = 0,
    ENUM_MSG_TYPE_REGIST_REQUEST,//注册请求
    ENUM_MSG_TYPE_REGIST_RESPOND,//注册回复

    ENUM_MSG_TYPE_LOGIN_REQUEST,//登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,//登录回复
    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,  //在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND,   //在线用户回复


    ENUM_MSG_TYPE_SEARCH_USR_REQUEST,   //搜索用户请求
    ENUM_MSG_TYPE_SEARCH_USR_RESPOND,   //搜索用户回复

    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,   //添加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,   //添加好友回复

    ENUM_MSG_TYPE_ADD_FRIEND_AGGREE,    //添加好友同意
    ENUM_MSG_TYPE_ADD_FRIEND_REFUSE,    //添加好友拒绝

    ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST, //刷新好友请求
    ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND, //刷新好友回复

    ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST, //删除好友请求
    ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND,  //删除好友回复

    ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST, //私聊请求
    ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND,  //私聊回复

    ENUM_MSG_TYPE_GROUP_CHAT_REQUEST, //群聊请求
    ENUM_MSG_TYPE_GROUP_CHAT_RESPOND,  //群聊回复

    ENUM_MSG_TYPE_CREATE_DIR_REQUEST,   //创建文件夹请求
    ENUM_MSG_TYPE_CREATE_DIR_RESPONE,   //创建文件夹回复

    ENUM_MSG_TYPE_FLUSH_FILE_REQUEST,   //刷新文件请求
    ENUM_MSG_TYPE_FLUSH_FILE_RESPONE,   //刷新文件回复

    ENUM_MSG_TYPE_DEL_DIR_REQUEST,   //删除文件请求
    ENUM_MSG_TYPE_DEL_DIR_RESPONE,   //删除文件回复

    ENUM_MSG_TYPE_RENAME_FILE_REQUEST,   //重命名文件请求
    ENUM_MSG_TYPE_RENAME_FILE_RESPONE,   //重命名文件回复

    ENUM_MSG_TYPE_ENTER_DIR_REQUEST,    //进去文件夹请求
    ENUM_MSG_TYPE_ENTER_DIR_RESQONE,    //进去文件夹回复

    ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST,   //上传文件请求
    ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND,   //上传文件回复

    ENUM_MSG_TYPE_DELETE_FILE_REQUEST,   //删除文件请求
    ENUM_MSG_TYPE_DELETE_FILE_RESPOND,   //删除文件回复

    ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST,   //下载文件请求
    ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND,   //下载文件回复

    ENUM_MSG_TYPE_SHARE_FILE_REQUEST,   //分享文件请求
    ENUM_MSG_TYPE_SHARE_FILE_RESPOND,   //分享文件回复

    ENUM_MSG_TYPE_SHARE_FILE_NOTE_REQUEST,   //分享文件通知请求
    ENUM_MSG_TYPE_SHARE_FILE_NOTE_RESPOND,   //分享文件通知回复

    ENUM_MSG_TYPE_MOVE_FILE_REQUEST,   //移动文件请求
    ENUM_MSG_TYPE_MOVE_FILE_RESPOND,   //移动文件回复

    ENUM_MSG_TYPE_MAX = 0x00ffffff

};

struct FileInfo
{
   char caFileName[32]; // 文件名字
   int iFileType;   //文件类型
};

struct PDU{
    uint uiPDULen; //总的数据单元大小
    uint uiMsgType; //消息类型
    char caData[64];
    uint uiMsgLen; //实际消息长度
    int caMsg[]; //实际消息
};
PDU *mkPDU(uint uiMsgLen);

#endif // PROTOCAL_H
