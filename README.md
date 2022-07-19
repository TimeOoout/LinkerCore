# LinkerCore
### 引言

* ~~本库旨在通过一个通用的软件后台管理模板，来减少开发者的工作量~~
* 本库为一个小白基于Qt写的软件后台管理模板
> 本库暂时还在制作中，尽情期待！\
> 开发进度：约5%
  ***
### 使用方法：
 * 仅需包括头文件***LinkerCore.h***即可
  
#### 内置函数
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
>> 当***confirm=true***时，settings将会写入到config.json中，否则仅初始化项目设置\

* QString ***get_setting***(QString key)
> key 为需要查询的设置值的键
> 如果未初始化设置，则返回 ""（空字符串)

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

* int ***login***(QString username, QString psw = "")
>[返回值说明]\
>1.<0>:运行正常\
>2.<-1>:未打开数据库\
>3.<-2>:未初始化\
>4.<-3>:未登录成功(密码或用户名错误)
>>与***reg***相同，若密码为空或未填则默认***无密码模式***\
>>***用户名不可为空***

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
    /* init函数 */
    LC.init(MainPath);
    /* reg函数 */
    LC.reg("测试账号");
    /* login函数 */
    LC.login("测试账号");
}
```




#### 其它注意事项
* 默认的项目数据/用户数据存放在系统用户的***文档***路径下
* 这也意味着该系统暂时***只支持Windows系统***
* 项目设置暂时只有 ***init***,***init_settings*** 以及 ***get_settings***三个函数涉及到，不初始化设置并没有问题，但系统仍然会在项目数据的目录下生成config.json
  
>作者写的并不好，一部分代码可以根据自己的需要进行修改，但请确定您***已经读懂了代码的逻辑***\
>一些可以修改的地方之后可能会单独封装一下
