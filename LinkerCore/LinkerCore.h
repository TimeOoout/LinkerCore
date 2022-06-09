#pragma once
#include <string>
#include <iostream>


#include <qdebug.h>
#include <qstandardpaths.h>
#include <qdir.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qjsonvalue.h>

/*
* [函数返回类型]
* #1.1 int类型
* 1.return 0
* 没有问题，正常运行
* 2 reuturn 1
* 运行正常，但部分任务未执行（看函数功能）
* 3 return -1
* 运行故障
* 
* 
* 
*/





namespace LinkerCore
{
	class LinkerCore
	{
	private:
		bool inited = false;
	public:
		std::string project;
		int inited_result = -3;
		bool inited_setting = false;
		//数据文件主路径
		QString MainPath;
		//config.json路径
		QString ConfigPath;
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
		//项目数据路径
		QJsonObject Paths;
		//Json文档
		QJsonDocument Jsdoc;

		//初始化类
		int init(QJsonObject pro_path);
		//项目设置
		int init_settings(QJsonObject settingsl, bool confirm);
		//创建用户
		int reg(QString username, QString psw);
		//获取设置
		QString get_setting(QString key);
		


	};
}


//初始化
int LinkerCore::LinkerCore::init(QJsonObject pro_path)

{
	if (inited == false)
	{
		//配置文件操作
		
		
		try
		{
			//数据文件主路径
			MainPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/";
			MainPath.append(pro_path.value("Parent").toString());
			MainPath.append("/");
			MainPath.append(pro_path.value("Child").toString());
			//config.json路径
			ConfigPath = MainPath.append("/config.json");
			//获取项目文件路径
			Pro_path = pro_path;
			//配置文件
			QFile file_config(ConfigPath);
			//若文件不存在
			if (!File.exists(ConfigPath))
			{
				
				try
				{
					//config文件打开
					file_config.open(QIODevice::WriteOnly);
					if (inited_setting == true)
					{
						Jsdoc.setObject(Settings);
						file_config.write(Jsdoc.toJson());
						file_config.close();
						Current_settings = Jsdoc.object();
						inited_result = 0;
						inited = true;
						return 0;
					}

					inited = true;
					//初始化但没有项目设置
					inited_result = 1;
					file_config.close();
					inited = true;
					return 1;
				}
				catch (const std::exception&)
				{
					if (file_config.isOpen())
					{
						file_config.close();
					}
					inited_result = -1;
					return -1;
				}
				
			}
			else
			{
				file_config.open(QIODevice::ReadOnly);
				QByteArray config_data = file_config.readAll();
				Jsdoc = QJsonDocument::fromJson(config_data);
				Current_settings = Jsdoc.object();
			}
			inited = true;
		}
		catch (const std::exception&)
		{
			
			//初始化异常
			inited_result = -1;
			return -1;
		}
		
	}
	//初始化成功/已初始化
	inited_result = 0;
	inited = true;
	return 0;
}
//初始化设置
int LinkerCore::LinkerCore::init_settings(QJsonObject settings,bool confirm=false)
{
	Settings = settings;
	inited_setting = true;
	if (confirm == false)
	{
		//初始化但没有写入
		return 1;
	}
	else
	{
		try
		{
		file_config.open(QIODevice::WriteOnly);
		Jsdoc.setObject(Settings);
		file_config.write(Jsdoc.toJson());
		file_config.close();
		}
		catch (const std::exception&)
		{
			if (file_config.isOpen())
			{
				file_config.close();
			}
			return -1;
		}

		return 0;
	}
	return 0;
}
//获取设置
QString LinkerCore::LinkerCore::get_setting(QString key)
{
	if (inited)
	{
		return Current_settings.value(key).toString();
	}
	else
	{
		return "";
	}
}
//创建账号
int LinkerCore::LinkerCore::reg(QString username, QString psw = "")
{
	if (psw == "")
	{
		return 0;
	}
	return 0;
}
