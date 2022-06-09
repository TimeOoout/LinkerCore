#include "LinkerCore.h"
#include "LinkerProject.h"

int main(void)
{
	LinkerCore::LinkerCore LC;
	LinkerProject::Linker Linker_pro;
	Linker_pro.init();
	LC.init_settings(Linker_pro.Settings);
	qDebug() << LC.init(Linker_pro.MainPath) << endl;
	qDebug() << LC.get_setting("Project") << endl;
	return 0;
} 