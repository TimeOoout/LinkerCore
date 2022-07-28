#include "LinkerCore.h"
#include "LinkerProject.h"




int main(int argc, char* argv[])
{
	LinkerCore LC;
	LinkerProject::Linker Linker_pro;
	Linker_pro.init();
	LC.init_settings(Linker_pro.Settings);
	qDebug() << QString::fromLocal8Bit("³õÊ¼»¯:") << LC.init(Linker_pro.MainPath) << endl;
	/*qDebug() << QString::fromLocal8Bit("²âÊÔ»ñÈ¡ÉèÖÃ:") << LC.get_setting("Project") << endl;
	qDebug() << QString::fromLocal8Bit("²âÊÔ×¢²á:" )<< LC.reg("²âÊÔ") << endl;
	qDebug() << QString::fromLocal8Bit("²âÊÔµÇÂ¼:") << LC.login("²âÊÔ") << endl;
	qDebug() << QString::fromLocal8Bit("²âÊÔ´´½¨ÓÃ»§×é'ÆÕÍ¨':") << LC.create_usergroup("ÆÕÍ¨") << endl;
	qDebug() << QString::fromLocal8Bit("²âÊÔÌí¼ÓÓÃ»§µ½ÓÃ»§×é'ÆÕÍ¨':") << LC.add_user("²âÊÔ", "ÆÕÍ¨") << endl;
	qDebug() << LC.get_groupusers("ÆÕÍ¨");
	qDebug() << LC.get_usergroups("²âÊÔ") << endl;
	qDebug() << QString::fromLocal8Bit("²âÊÔÉ¾³ýÓÃ»§'²âÊÔ':") << LC.delete_user("²âÊÔ") << endl;
	qDebug() << QString::fromLocal8Bit("²âÊÔ×éÄÚÉ¾³ýÓÃ»§'²âÊÔ':") << LC.remove_user("²âÊÔ","ÆÕÍ¨") << endl;*/


	LC.write_log("³õÊ¼»¯:" + QString::number(LC.inited_result).toStdString());
	LC.write_log("²âÊÔ×¢²á:" + QString::number(LC.reg("²âÊÔ")).toStdString());
	LC.write_log("²âÊÔµÇÂ¼:" + QString::number(LC.login("²âÊÔ")).toStdString());
	LC.write_log("²âÊÔ´´½¨ÓÃ»§×é'ÆÕÍ¨':"+ QString::number(LC.create_usergroup("ÆÕÍ¨")).toStdString());
	LC.write_log("²âÊÔÌí¼ÓÓÃ»§µ½ÓÃ»§×é'ÆÕÍ¨':" + QString::number(LC.add_user("²âÊÔ", "ÆÕÍ¨")).toStdString());
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

