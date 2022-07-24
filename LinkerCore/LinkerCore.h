#pragma once

#include <string>
#include <iostream>

#include <qdatastream.h>
#include <qdebug.h>
#include <qstandardpaths.h>
#include <qdir.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qjsonvalue.h>
#include <quuid.h>
#include <qcryptographichash.h>
#include <qtextcodec.h>
#include <QtSql\qsqldatabase.h>
#include <QtSql\qsqlerror.h>
#include <QtSql\qsqlquery.h>
#include <QtSql\qsqlrecord.h>



class LinkerCore
{
private:
	//是否初始化
	bool inited = false;
	//是否登录
	bool logined = false;
	//是否初始化设置
	bool inited_setting = false;
	//初始化结果
	int inited_result = -3;
	//是否使用密码
	bool use_pw;


public:
	//数据库操作
	QSqlDatabase db;
	//数据文件主路径
	QString MainPath;
	//config.json路径
	QString ConfigPath;
	//用户文件路径
	QString UserFilePath;
	//日志路径
	QString LogPath;
	//组件路径
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

	//初始化类
	int init(QJsonObject pro_path)
	{
		/*
		[返回值说明]
		1.<0>:运行正常
		2.<-1>:未成功打开config.json
		3.<-2>:未知错误
		*/
		if (inited == false)
		{

			try
			{
				//数据文件主路径
				MainPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/";
				MainPath.append(pro_path.value("Parent").toString());
				if (!File.exists(MainPath))
				{
					Dir.mkdir(MainPath);
				}
				MainPath.append("/");
				MainPath.append(pro_path.value("Child").toString());
				if (!File.exists(MainPath))
				{
					Dir.mkdir(MainPath);
				}
				//config.json路径
				ConfigPath = MainPath + "/config.json";
				//用户文件路径
				UserFilePath = MainPath + "/user";
				//日志路径
				LogPath = MainPath + "/log";
				//组件路径
				PackagePath = MainPath + "/package";
				//获取项目文件路径
				Pro_path = pro_path;
				//配置文件
				QFile file_config(ConfigPath);
				
				if (!File.exists(UserFilePath))
				{
					Dir.mkdir(UserFilePath);
				}
				if (!File.exists(LogPath))
				{
					Dir.mkdir(LogPath);
				}
				if (!File.exists(PackagePath))
				{
					Dir.mkdir(PackagePath);
				}

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
						//未成功打开config.json
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
				//其它错误
				inited_result = -2;
				return -2;
			}

		}
		//初始化成功/已初始化
		inited_result = 0;
		inited = true;
		return 0;
	}
	/*
	init(QJsonObject project_path)
	将会初始化
	{
	//数据文件主路径
	QString MainPath;

	//config.json路径
	QString ConfigPath;

	//用户文件路径
	QString UserFilePath;

	//日志路径
	QString LogPath;

	//组件路径
	QString PackagePath;

	//config文件
	QFile file_config;

	//项目路径组
	QJsonObject Pro_path;
	}
	**注意：以下是可能会初始化的，具体见返回值
	{
	//项目初始设置
	QJsonObject Settings;

	//项目当前设置
	QJsonObject Current_settings;

	}
	*/
	//项目设置
	int init_settings(QJsonObject settings, bool confirm = false)
	{
		/*
		[返回值说明]
		1.<0>
		运行正常
		2.<1>
		初始化但没有写入
		3.<-1>
		未成功写入
		*/
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
	QString get_setting(QString key)
	{
		if (inited_setting=true)
		{
			return Current_settings.value(key).toString();
		}
		else
		{
			return "";
		}
	}
	//创建用户
	int reg(std::string Username, std::string Psw = "")
	{
		/*
		[返回值说明]
		1.<0>:运行正常
		2.<-1>:未打开数据库
		3.<-2>:未初始化
		4.<-3>:插入数据出错/已注册
		*/
		if (inited == false)
		{
			return -2;
		}
		else
		{

			if (QSqlDatabase::contains("Users"))
			{
				db = QSqlDatabase::database("Users");
			}	
			else
			{
				db = QSqlDatabase::addDatabase("QSQLITE","Users");
				db.setDatabaseName(UserFilePath + "/Users.lsf");
			}
			//打开数据库
			db.open();

			if (!db.isOpen())
			{
				//未打开用户数据库
				return -1;
			}
			QSqlQuery sql_query(db);
			QString psw = QString::fromLocal8Bit(Psw.c_str());
			QString username = QString::fromLocal8Bit(Username.c_str());

			QString cmd;


			if (psw == "" && username != "")
			{
				cmd = "create table if not exists Users ( Name VARCHAR(10), UUID VARCHAR(56), PRIMARY KEY(UUID))";
				sql_query.prepare(cmd);
				if (!sql_query.exec())
				{
					//qDebug() << "Error: Fail to create table." << sql_query.lastError();
					db.close();
					return -2;
				}
				cmd = "insert into Users values(?,?)";
				sql_query.prepare(cmd);
				sql_query.addBindValue(username);
				sql_query.addBindValue((QString)QCryptographicHash::hash(username.toLatin1(), QCryptographicHash::Sha3_224).toHex());
			}
			else if (psw != "" && username != "")
			{
				cmd = "create table if not exists Users ( Name VARCHAR(10), UUID VARCHAR(56), Password VARCHAR(64), PRIMARY KEY(UUID))";
				sql_query.prepare(cmd);
				if (!sql_query.exec())
				{
					//qDebug() << "Error: Fail to create table." << sql_query.lastError();
					db.close();
					return -2;
				}
				cmd = "insert into Users values(?,?,?)";
				sql_query.prepare(cmd);
				sql_query.addBindValue(username);
				sql_query.addBindValue((QString)QCryptographicHash::hash(username.toLatin1(), QCryptographicHash::Sha3_224).toHex());
				sql_query.addBindValue((QString)QCryptographicHash::hash(psw.toLatin1(), QCryptographicHash::Sha3_256).toHex());
			}
			if (!sql_query.exec())
			{
				//qDebug() << sql_query.lastError() << endl;
				db.close();
				return -3;
			}
			db.close();
			return 0;
		}

	}
	//登录 
	int login(std::string Username, std::string Psw = "")
	{
		/*
		[返回值说明]
		1.<0>:运行正常
		2.<-1>:未打开数据库
		3.<-2>:未初始化
		4.<-3>:未登录成功(密码或用户名错误)
		*/
		//判断是否初始化
		if (inited == false)
		{
			return -2;
		}
		//判断是否存在Users连接
		if (QSqlDatabase::contains("Users"))
		{
			db = QSqlDatabase::database("Users");
		}
		else
		{
			db = QSqlDatabase::addDatabase("QSQLITE", "Users");
			db.setDatabaseName(UserFilePath + "/Users.lsf");
		}
		//打开数据库
		db.open();
		if (!db.isOpen())
		{
			//未打开用户数据库的情况
			return -1;
		}
		QSqlQuery sql_query(db);
		QString psw = QString::fromLocal8Bit(Psw.c_str());
		QString username = QString::fromLocal8Bit(Username.c_str());
		if (psw == "" && username != "")
		{
			QString cmd = "select * from Users where Name='"+ username +"' and UUID='"+ (QString)QCryptographicHash::hash(username.toLatin1(), QCryptographicHash::Sha3_224).toHex()+"'";
			sql_query.prepare(cmd);
			if (!sql_query.exec())
			{
				//qDebug() << "Error: Fail to select." << sql_query.lastError();
				db.close();
				return -2;
			}
			if (!sql_query.next())
			{
				db.close();
				return -3;
			}
			db.close();
			return 0;
		}
		else if (psw != "" && username != "")
		{
			QString cmd = "select * from Users where Name='" + username + "' and UUID='" + (QString)QCryptographicHash::hash(username.toLatin1(), QCryptographicHash::Sha3_224).toHex() + "' and Password='" + (QString)QCryptographicHash::hash(psw.toLatin1(), QCryptographicHash::Sha3_256).toHex() + "'";
			sql_query.prepare(cmd);
			if (!sql_query.exec())
			{
				//qDebug() << "Error: Fail to select." << sql_query.lastError();
				db.close();
				return -2;
			}
			if (!sql_query.next())
			{
				db.close();
				return -3;
			}
			db.close();
			return 0;
		}
		db.close();
		return 0;
	


	}
	//查看所有用户 *[返回QJsonObject user_list]
	QJsonObject show_users(void)
	{
		QJsonObject get;
		if (inited == false)
		{
			return get;
		}
		//判断是否存在Users连接
		if (QSqlDatabase::contains("Users"))
		{
			db = QSqlDatabase::database("Users");
		}
		else
		{
			db = QSqlDatabase::addDatabase("QSQLITE", "Users");
			db.setDatabaseName(UserFilePath + "/Users.lsf");
		}
		//打开数据库
		db.open();
		if (!db.isOpen())
		{
			//未打开用户数据库的情况
			return get;
		}
		QSqlQuery query(db);
		query.exec("select * from Users");
		QSqlRecord rec = query.record();
		QString name;
		QString uuid;
		while (query.next())
		{
			rec = query.record();
			uuid = query.value(rec.indexOf("UUID")).toString();
			name = query.value(rec.indexOf("Name")).toString();
			//qDebug() << "UserName:" << name;
			//qDebug() << "UUID:" << uuid;
			get.insert(name,uuid);

		}
		db.close();
		user_list = get;
		return get;
	}
	//删除用户
	int delete_user(std::string Username, std::string Psw = "")
	{
		/*
		[返回值说明]
		1.<0>:运行正常
		2.<-1>:未成功打开用户数据库
		3.<-2>:未初始化
		4.<-3>:未成功删除/其他错误
		*/
		if (inited == false)
		{
			return -2;
		}
		//判断是否存在Users连接
		if (QSqlDatabase::contains("Users"))
		{
			db = QSqlDatabase::database("Users");
		}
		else
		{
			db = QSqlDatabase::addDatabase("QSQLITE", "Users");
			db.setDatabaseName(UserFilePath + "/Users.lsf");
		}
		//打开数据库
		db.open();
		if (!db.isOpen())
		{
			//未打开用户数据库的情况
			return -1;
		}
		QSqlQuery query(db);
		QString username = QString::fromLocal8Bit(Username.c_str());
		QString psw = QString::fromLocal8Bit(Psw.c_str());
		if (Username != "" && psw == "")
		{
			if (query.exec("delete from Users where Name='" + username + "' and UUID='" + (QString)QCryptographicHash::hash(username.toLatin1(), QCryptographicHash::Sha3_224).toHex() + "'"))
			{
				db.close();
				show_users();
				if (user_list.value(username) == (QString)QCryptographicHash::hash(username.toLatin1(), QCryptographicHash::Sha3_224).toHex())
				{
					//未成功删除
					return -3;
				}
				return 0;
			}
		}
		else if (Username != "" && psw != "")
		{
			if (query.exec("delete from Users where Name='" + username + "' and UUID='" + (QString)QCryptographicHash::hash(username.toLatin1(), QCryptographicHash::Sha3_224).toHex() + "' and Password='" + (QString)QCryptographicHash::hash(psw.toLatin1(), QCryptographicHash::Sha3_256).toHex() + "'"))
			{
				db.close();
				show_users();
				if (user_list.value(username) == (QString)QCryptographicHash::hash(username.toLatin1(), QCryptographicHash::Sha3_224).toHex())
				{
					//未成功删除
					return -3;
				}
				return 0;
			}
		}
		db.close();
		return -3;
	}


};