#include "LinkerCore.h"
#include "LinkerProject.h"




int main(int argc, char* argv[])
{
	LinkerCore LC;
	LinkerProject::Linker Linker_pro;
	Linker_pro.init();
	LC.init_settings(Linker_pro.Settings);
	qDebug() << QString::fromLocal8Bit("³õÊ¼»¯:") << LC.init(Linker_pro.MainPath) << endl;
	qDebug() << QString::fromLocal8Bit("²âÊÔ»ñÈ¡ÉèÖÃ:") << LC.get_setting("Project") << endl;
	qDebug() << QString::fromLocal8Bit("²âÊÔ×¢²á:" )<< LC.reg("²âÊÔÕËºÅ") << endl;
	qDebug() << QString::fromLocal8Bit("²âÊÔµÇÂ¼:") << LC.login("²âÊÔÕËºÅ") << endl;
} 

