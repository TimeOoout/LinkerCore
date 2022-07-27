#include "LinkerCore.h"
#include "LinkerProject.h"




int main(int argc, char* argv[])
{
	LinkerCore LC;
	LinkerProject::Linker Linker_pro;
	Linker_pro.init();
	LC.init_settings(Linker_pro.Settings);
	qDebug() << QString::fromLocal8Bit("初始化:") << LC.init(Linker_pro.MainPath) << endl;
	qDebug() << QString::fromLocal8Bit("测试获取设置:") << LC.get_setting("Project") << endl;
	qDebug() << QString::fromLocal8Bit("测试注册:" )<< LC.reg("测试") << endl;
	qDebug() << QString::fromLocal8Bit("测试登录:") << LC.login("测试") << endl;
	qDebug() << QString::fromLocal8Bit("测试创建用户组'普通':") << LC.create_usergroup("普通");
	qDebug() << QString::fromLocal8Bit("测试添加用户到用户组'普通':") << LC.add_user("测试", "普通");
	qDebug() << QString::fromLocal8Bit("测试删除用户'测试':") << LC.delete_user("测试") << endl;
	qDebug() << LC.get_users() << endl;

} 

