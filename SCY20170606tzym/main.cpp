#include "frmmain.h"
#include <QTextCodec>
#include <QTranslator>
#include <QDesktopWidget>
#include "api/myapp.h"
#include "api/myhelper.h"
#include "api/myapi.h"
#include <QFont>
#if QT_VERSION >= 0x050000
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif
#include "frminput.h"
#include "frmnum.h"
#include <stdio.h>
#include <stdlib.h>
static QSqlDatabase        DbConn;
void myMessageOutput(QtMsgType type, const char *msg);
myAPI api_main;

int main(int argc, char *argv[])
{
    int res=0;
    QApplication a(argc, argv);
    system("echo start[`date`] >> /mnt/nandflash/start.log");
    QFont font("SIMSUN",9,QFont::Normal);           //设置字体
    a.setFont(font);
    qInstallMsgHandler(myMessageOutput);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");      //中文字体编码方式
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    //赋值当前应用程序路径和桌面宽度高度
    myApp::AppPath=QApplication::applicationDirPath()+"/";
    myApp::DeskWidth=qApp->desktop()->availableGeometry().width();
    myApp::DeskHeigth=qApp->desktop()->availableGeometry().height();

    //判断当前数据库文件是否存在(如果数据库打开失败则终止应用程序)
//    if (!myHelper::FileIsExist(myApp::AppPath+"db_scy.db")){
       if (!myHelper::FileIsExist("/mnt/sdcard/db_scy.db")){

        myHelper::ShowMessageBoxError("数据库文件不存在,程序将自动关闭！");
        return 1;
    }
    QSqlDatabase DbConn;                //
    DbConn=QSqlDatabase::addDatabase("QSQLITE");
//    DbConn.setDatabaseName(myApp::AppPath+"db_scy.db");
    DbConn.setDatabaseName("/mnt/sdcard/db_scy.db");
    //创建数据库连接并打开(如果数据库打开失败则终止应用程序)
    if (!DbConn.open()){
        myHelper::ShowMessageBoxError("打开数据库失败,程序将自动关闭！");
        return 1;
    }
    else{
        if(!api_main.TableIsExist("LastHour")){
        QSqlQuery query;
        if(query.exec("create table LastHour(GetTime NVARCHAR(20),Flow NVARCHAR(20));")){
            qDebug()<<QString(" insert into [LastHour] ([GetTime],[Flow]) values('%1','--')").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:00"));
            query.exec(QString(" insert into [LastHour] ([GetTime],[Flow]) values('%1','--')").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:00")));

        }
        query.clear();
        }
    }

    QSqlDatabase memorydb;
    memorydb=QSqlDatabase::addDatabase("QSQLITE","memory");
    //如果要连接多个数据库，必须指定不同的连接名字，否则会覆盖上面的默认连接
    memorydb.setDatabaseName(":memory:");
    if(!memorydb.open()){
        myHelper::ShowMessageBoxError("打开内部数据库失败，程序将自动关闭！");
    }
    else
    {
        QSqlQuery query_memory(QSqlDatabase::database("memory",true));
        query_memory.exec("create table CacheRtd(GetTime DATETIME(20),Name NVARCHAR(20),Code NVARCHAR(10),Rtd NVARCHAR(20),Total NVARCHAR(20),Flag NVARCHAR(5),ErrorNums NVARCHAR(2));");
        query_memory.clear();
    }
    //程序加载时先加载所有配置信息
//    try{
    myApp::ReadConfig();
    myApp::ReadIoConfig();

//    if(!myHelper::MD5_EncryptDecrypt(myApp::Key))//防止应用拷贝,在其他设备上无法运行
//    {
//        myHelper::ShowMessageBoxError("本机无法正常运行！");
//        return 1;
//    }

    frmMain w;
    w.showFullScreen();


    //在main函数中就加载输入法面板,保证整个应用程序可以使用输入法
    //以下方法打开中文输入法
    frmInput::Instance()->Init("control", "black", 10, 10);
    //以下方法打开数字键盘
    //frmNum::Instance()->Init("black", 10);

        api_main.AddEventInfoUser("系统启动运行");

    res=a.exec();
    DbConn.close();
    QString name;
    {
        name = QSqlDatabase::database().connectionName();
    }//超出作用域，隐含对象QSqlDatabase::database()被删除。
    QSqlDatabase::removeDatabase(name);
    system("echo stop[`date`] >> /mnt/nandflash/start.log");
    return res;

}

void myMessageOutput(QtMsgType type, const char *msg)
 {
     switch (type) {
     case QtDebugMsg:
         fprintf(stderr, "Debug: %s\n", msg);
         break;
     case QtWarningMsg:
         fprintf(stderr, "Warning: %s\n", msg);
         break;
     case QtCriticalMsg:
         fprintf(stderr, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         fprintf(stderr, "Fatal: %s\n", msg);
         abort();
     }
 }
