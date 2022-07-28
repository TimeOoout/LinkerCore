# LinkerCore
### 引言

* ~~本库旨在通过一个通用的软件后台管理模板，来减少开发者的工作量~~
* 本库为一个小白基于Qt写的软件后台管理模板
* 最新版本 LinkerCore v0.1.1_220728beta
> 后续会提供更多功能，尽请期待！\
> 目前功能已经可以正常使用！\
> 一些功能可以自行开发，类中可以自由使用的一些对象放在最后了
***
### 使用方法：
 * 仅需包括头文件***LinkerCore.h***即可

### 注意事项
* 因为是使用数据库进行操作，所以会有SQL注入的风险，在使用时需注意**数据合法性判断**
* 本库面对的是一些较为简单的单机用户数据储存，暂时无法保证数据安全性，并且**不支持异步**
  
### 本次更新内容
* 允许**同时存在**使用密码和不使用密码的两种用户模式
* 添加了用户组功能，可以根据项目需要添加用户组
* 添加了日志功能
* 添加了备份功能，每次程序结束后自动备份用户以及用户组数据
* 修改了一些表述错误
* 修复了一些已知的BUG

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
> 3.<-2>:未知错误\
> 4.<-3>:未成功创建用户数据库
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
>4.<-3>:插入数据出错\
>5.<-4>:未成功创建用户组关联表\
>6.<-5>:已注册
>>本系统同时支持带密码的用户模式和无密码的用户模式\
>>若未填密码或为空字符串都默认密码为空字符串\
>>***用户名不可为空、用户名不可重复!***\

***
#### ***登录***
* int ***login***(QString username, QString psw = "")
>[返回值说明]\
>1.<0>:运行正常\
>2.<-1>:未打开数据库\
>3.<-2>:未初始化\
>4.<-3>:未登录成功(密码或用户名错误)\
>5.<-4>:未成功打开数据库搜索
>>与***reg***相同,***用户名不可为空***

***
#### ***获取用户列表***
* QJsonObject ***get_users***(void)

>若未初始化或未打开数据库，则返回一个空的QJsonObject\
>否则将返回一个包含所有用户用户名以及UUID的QJsonObject

*** 
#### ***修改密码***
* int ***chg_psw***(std::string Username, std::string Psw="", std::string NewPsw = "")

>[返回值说明]\
>1.<0>:运行正常\
>2.<-1>:未成功打开用户数据库\
>3.<-2>:未初始化\
>4.<-3>:未成功修改\
>5.<-4>:不存在该用户


***
#### ***删除用户***
* int ***delete_user***(std::string Username, std::string Psw = "")

>[返回值说明]
>1.<0>:运行正常\
>2.<-1>:未成功打开用户数据库\
>3.<-2>:未初始化\
>4.<-3>:未成功删除/其他错误
>5.<-4>:不存在该用户
>6.<-5>:密码错误
>>与reg相同，若密码为空或未填则默认密码为空字符串\
>>***用户名不可为空***\
>>删除用户时会自动将用户从用户所在的用户组删除

***
#### ***创建用户组***
* int ***create_usergroup***(std::string GroupName)
  
>[返回值说明]\
>1.<0>:运行正常\
>2.<-1>:未成功打开数据库\
>3.<-2>:未初始化\
>4.<-3>:未成功创建用户组
>>***用户组名不可为空***\
>>系统本身并不涉及用户组操作，此处用户组是为了项目方便而设计的，**并不是必需品**

***
#### ***向用户组内添加用户***
* int ***add_user***(std::string Username, std::string GroupName)

>[返回值说明]\
>1.<0>:运行正常\
>2.<-1>:未成功打开数据库\
>3.<-2>:未初始化\
>4.<-3>:不存在该用户\
>5.<-4>:未成功添加用户\
>6.<-5>:已注册 \
>7.<-6>:未成功向用户组关联表添加组

***
#### ***从组内删除用户***
* int ***remove_user***(std::string Username, std::string GroupName)
  
>[返回值说明]\
>1.<0>:运行正常\
>2.<-1>:未成功打开数据库\
>3.<-2>:未初始化\
>4.<-3>:不存在该用户\
>5.<-4>:未成功删除该用户\
>6.<-5>:未成功删除记录

***
#### ***查看组内用户***
* QJsonObject ***get_groupusers***(std::string GroupName)

>若未初始化或未打开数据库，则返回一个空的QJsonObject\
>否则将返回一个包含该组内用户用户名以及UUID的QJsonObject

***
#### ***查看用户关联的用户组***
* QJsonObject ***get_usergroups***(std::string Username)
>若未初始化或未打开数据库，则返回一个空的QJsonObject\
>否则将返回一个包含该用户加入用户组顺序的ID以及用户所在用户组名的QJsonObject

***
#### ***插入日志***
* int ***write_log***(std::string Info,std::string Utils="Utils" , std::string Type="INFO")

>[返回值说明]\
>1.<0>:运行正常\
>2.<-1>:未成功打开日志文件\
>3.<-2>:未初始化
>>默认日志路径为: 文档/路径项目父文件夹/项目子文件夹/log/\
>>文档的路径一般为: C:\Users\用户名\Documents




#### 示例：
##### 1.初始化/注册/登录/创建用户组&添加用户/修改密码/删除组内用户/删除用户/日志写入
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
    //以下输出请在日志中查看
    LC.write_log("初始化:" + QString::number(LC.inited_result).toStdString());
	LC.write_log("测试注册:" + QString::number(LC.reg("测试", "密码")).toStdString());
	LC.write_log("测试登录:" + QString::number(LC.login("测试","密码")).toStdString());
	LC.write_log("测试创建用户组'普通':"+ QString::number(LC.create_usergroup("普通")).toStdString());
	LC.write_log("测试添加用户到用户组'普通':" + QString::number(LC.add_user("测试", "普通")).toStdString());
	LC.write_log("测试修改密码:" + QString::number(LC.chg_psw("测试", "密码")).toStdString());
	LC.write_log("测试删除用户'测试':" + QString::number(LC.delete_user("测试")).toStdString());
	LC.write_log("测试组内删除用户'测试':" + QString::number(LC.remove_user("测试", "普通")).toStdString());
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
