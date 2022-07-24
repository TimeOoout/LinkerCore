# LinkerCore
### 引言

* ~~本库旨在通过一个通用的软件后台管理模板，来减少开发者的工作量~~
* 本库为一个小白基于Qt写的软件后台管理模板
* 因为是使用数据库进行操作，所以会有SQL注入的风险，在使用时需注意数据合法性判断
* 本库面对的是一些较为简单的单机用户数据储存，暂时无法保证数据安全性
> 本库暂时还在制作中，尽情期待！\
> 目前功能已经可以正常使用\
> 后续将实现日志储存以及用户其它数据储存\
> 一些功能可以自行开发，类中的一些对象放在最后了\
> 开发进度：约10%
***
### 使用方法：
 * 仅需包括头文件***LinkerCore.h***即可
  
### 内置函数
***
#### ***初始化***
* int ***init***(QJsonObject project_path)
```
project_path={
    "Parent":"Parent_folder",
    "Child":"Child_folder"
}
```
> [返回值说明]\
> 1.<0>:运行正常\
> 2.<-1>:未成功打开config.json\
> 3.<-2>:未知错误
***
#### ***初始化设置***
* int ***init_settings***(QJsonObject settings, bool confirm = false)
```
settings={
    "A":"a",
    "B":"b",
    ......
}
```

> [返回值说明]\
>1.<0>:运行正常\
>2.<1>:初始化但没有写入\
>3.<-1>:未成功写入
>> 此处settings（设置）的内容***和管理系统无关***，***仅供项目使用***\
>> 当***confirm=true***时，settings将会写入到config.json中，否则仅初始化项目设置

***
#### ***获取设置***
* QString ***get_setting***(QString key)
> key 为需要查询的设置值的键
> 如果未初始化设置，则返回 ""（空字符串)

***
#### ***注册用户***
* int ***reg***(QString username, QString psw = "")

>[返回值说明]\
>1.<0>:运行正常\
>2.<-1>:未打开数据库\
>3.<-2>:未初始化\
>4.<-3>:插入数据出错/已注册
>>本系统支持带密码的用户模式和无密码的用户模式，但同一项目下***二者不可混用***，***否则将无法正常登录/注册***\
>>无密码模式或使用密码的用户模式取决于***第一次注册的用户***\
>>后续可能添加支持或检查机制\
>>若密码为空或未填则默认***无密码模式***\
>>***用户名不可为空、用户名不可重复***

***
#### ***登录***
* int ***login***(QString username, QString psw = "")
>[返回值说明]\
>1.<0>:运行正常\
>2.<-1>:未打开数据库\
>3.<-2>:未初始化\
>4.<-3>:未登录成功(密码或用户名错误)
>>与***reg***相同，若密码为空或未填则默认***无密码模式***\
>>***用户名不可为空***

* QJsonObject show_users(void)

>若未初始化或未打开数据库，则返回一个空的QJsonObject\
>否则将返回一个包含所有用户用户名以及UUID的QJsonObject

***
#### ***删除用户***
* int delete_user(std::string Username, std::string Psw = "")

>[返回值说明]
>1.<0>:运行正常\
>2.<-1>:未成功打开用户数据库\
>3.<-2>:未初始化\
>4.<-3>:未成功删除/其他错误
>>与reg相同，若密码为空或未填则默认***无密码模式***\
>>***用户名不可为空***\
>>只能删除用户，但***并不判断该用户是否存在***



#### 示例：
##### 1.初始化&注册/登录
```
#include "LinkerCore.h"
int main()
{
    LinkerCore LC;
    QJsonObject MainPath;
    //项目主路经
    MainPath.insert("Parent", "Linker");
    MainPath.insert("Child", "Linker");
    //初始化
    //使用fromLocal8Bit是为了避免乱码
    qDebug() << QString::fromLocal8Bit("初始化:") << LC.init(MainPath) << endl;
	qDebug() << QString::fromLocal8Bit("测试获取设置:") << LC.get_setting("Project") << endl;
	qDebug() << QString::fromLocal8Bit("测试注册:" )<< LC.reg("测试") << endl;
	qDebug() << QString::fromLocal8Bit("测试登录:") << LC.login("测试") << endl;
    //查看用户
	qDebug() << LC.show_users() << endl;
	qDebug() << QString::fromLocal8Bit("测试删除:") << LC.delete_user("测试");
	qDebug() << LC.show_users() << endl;
}
```

#### **可调用对象**
* ***请勿随意给对象赋值，否则可能出错***
* ***如果要修改整个项目的数据储存路径，请在 init 函数中修改***
```
//数据库操作
QSqlDatabase db;

//数据文件主路径
QString MainPath;

//config.json路径
QString ConfigPath;

//用户文件路径
QString UserFilePath;

//日志路径（暂未使用到）
QString LogPath;

//组件路径（暂未使用到）
QString PackagePath;

//通用文件操作
QFile File;

//config文件
QFile file_config;

//通用路径操作
QDir Dir;

//项目路径组
QJsonObject Pro_path;

//项目初始设置
QJsonObject Settings;

//项目当前设置
QJsonObject Current_settings;

//Json文档
QJsonDocument Jsdoc;

//由函数 show_users 赋值
QJsonObject user_list;
```


#### 其它注意事项
* 默认的项目数据/用户数据存放在系统用户的***文档***路径下,这也意味着本库暂时***只支持Windows系统***
* 项目设置暂时只有 ***init***,***init_settings*** 以及 ***get_settings***三个函数涉及到，不初始化设置并没有问题，但系统仍然会在项目数据的目录下生成config.json

>**作者写的并不好，一部分代码可以根据自己的需要进行修改，但请确定您*****已经读懂了代码的逻辑***\
>一些可以修改的地方之后可能会单独封装一下
