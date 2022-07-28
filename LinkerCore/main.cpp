#include "LinkerCore.h"
#include "LinkerProject.h"




int main(int argc, char* argv[])
{
	LinkerCore LC;
	LinkerProject::Linker Linker_pro;
	Linker_pro.init();
	LC.init_settings(Linker_pro.Settings);
	qDebug() << QString::fromLocal8Bit("³õÊ¼»¯:") << LC.init(Linker_pro.MainPath) << endl;
	LC.write_log("³õÊ¼»¯:" + QString::number(LC.inited_result).toStdString());
	LC.write_log("²âÊÔ×¢²á:" + QString::number(LC.reg("²âÊÔ", "ÃÜÂë")).toStdString());
	LC.write_log("²âÊÔµÇÂ¼:" + QString::number(LC.login("²âÊÔ","ÃÜÂë")).toStdString());
	LC.write_log("²âÊÔ´´½¨ÓÃ»§×é'ÆÕÍ¨':"+ QString::number(LC.create_usergroup("ÆÕÍ¨")).toStdString());
	LC.write_log("²âÊÔÌí¼ÓÓÃ»§µ½ÓÃ»§×é'ÆÕÍ¨':" + QString::number(LC.add_user("²âÊÔ", "ÆÕÍ¨")).toStdString());
	LC.write_log("²âÊÔÐÞ¸ÄÃÜÂë:" + QString::number(LC.chg_psw("²âÊÔ", "ÃÜÂë")).toStdString());
	qDebug() << LC.get_groupusers("ÆÕÍ¨");
	qDebug() << LC.get_usergroups("²âÊÔ") << endl;
	LC.write_log("²âÊÔÉ¾³ýÓÃ»§'²âÊÔ':" + QString::number(LC.delete_user("²âÊÔ")).toStdString());
	LC.write_log("²âÊÔ×éÄÚÉ¾³ýÓÃ»§'²âÊÔ':" + QString::number(LC.remove_user("²âÊÔ", "ÆÕÍ¨")).toStdString());

	/*
	 LC.init(Linker_pro.MainPath)
	 LC.reg("²âÊÔ")
	 LC.login("²âÊÔ")
	 LC.create_usergroup("ÆÕÍ¨")
	 LC.add_user("²âÊÔ", "ÆÕÍ¨")
	 LC.delete_user("²âÊÔ")
	 LC.remove_user("²âÊÔ","ÆÕÍ¨")
	*/
} 

