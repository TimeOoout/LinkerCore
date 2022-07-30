#pragma once

/*
** (C) Copyright 2022 Hang Jiayu. All Rights Reserved.
** Description:基于Qt的软件后台管理模板
** Author:Hang Jiayu
** Date:2022.07.28
** Modify Record:完善基本功能
*/

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
#include <qdatetime.h>
#include <qtextstream.h>



class LinkerCore
{
private:
	//是否初始化
	bool inited = false;
	//是否登录
	bool logined = false;
	//是否初始化设置
	bool inited_setting = false;
	
	//是否记录日志（默认开启
	bool Logging = true;

	int _remove_user(std::string Username, std::string GroupName)
	{
		/*
		[返回值说明]
		1.<0>:运行正常
		2.<-1>:未成功打开数据库
		3.<-2>:未初始化
		4.<-3>:不存在该用户
		5.<-4>:未成功删除该用户
		6.<-5>:未成功删除记录
		*/
		if (inited == false)
		{
			return -2;
		}
		//更新用户列表
		get_groupusers(GroupName);
		QString groupname = GroupName.c_str();
		QString username = QString::fromLocal8Bit(Username.c_str());
		if (groupuser_list.contains(username) == false)
		{
			return -3;
		}
		//判断是否存在UserGroup连接
		if (QSqlDatabase::contains("UserGroup"))
		{
			db = QSqlDatabase::database("UserGroup");
		}
		else
		{
			db = QSqlDatabase::addDatabase("QSQLITE", "UserGroup");
			db.setDatabaseName(UserFilePath + "/UserGroup.lsf");
		}
		//打开数据库
		db.open();
		if (!db.isOpen())
		{
			//未打开用户数据库的情况
			return -1;
		}
		QSqlQuery sql_query(db);
		if (!sql_query.exec("delete from " + groupname + " where Name='" + username + "' and UUID='" + groupuser_list.value(username).toString() + "'"))
		{
			//qDebug() << sql_query.lastError() << endl;
			db.close();
			return -4;
		}
		db.close();
		get_users();
		if (user_list.contains(username) == false)
		{
			return 0;
		}
		/************删除用户组记录************/
		{
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
				//未打开用户数据库
				return -1;
			}
			QSqlQuery query(db);
			if (!query.exec("delete from " + user_list.value(username).toString() + " where GroupName='" + groupname + "'"))
			{
				//qDebug() << query.lastError() << endl;
				db.close();
				return -5;
			}
		}
		return 0;
	}


public:
	//初始化结果
	int inited_result = -10086;
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
	QFile file_log;
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
	//当前时间 
	QDateTime curDateTime;

	//由函数 get_users 赋值
	QJsonObject user_list;
	QJsonObject user_psw;

	//由函数 get_groupusers 赋值[*动态变化]
	QJsonObject groupuser_list;
	QJsonObject usergroup_list;


	//初始化类
	int init(QJsonObject pro_path)
	{
		/*
		[返回值说明]
		1.<0>:运行正常
		2.<-1>:未成功打开config.json
		3.<-2>:未知错误
		4.<-3>:未成功创建用户数据库
		*/
		if (inited == false)
		{
			try
			{
				//数据文件主路径
				{
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
				}
				//config.json路径
				ConfigPath = MainPath + "/config.json";
				//用户文件路径
				UserFilePath = MainPath + "/user";
				//日志路径
				curDateTime = QDateTime::currentDateTime();
				LogPath = MainPath + "/log/" + curDateTime.toString("yyyy-MM-dd_hh-mm-ss") + ".log";
				file_log.setFileName(LogPath);
				Logging = true;
				if (Logging == true)
				{
					file_log.open(QIODevice::ReadWrite);
				}
				//组件路径
				PackagePath = MainPath + "/package";
				//获取项目文件路径
				Pro_path = pro_path;
				//配置文件
				QFile file_config(ConfigPath);
				//检查路径是否存在
				{
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
				}

				/**创建用户数据库**/
				{
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
						//未打开用户数据库
						inited_result = -1;
						return -1;
					}
					QSqlQuery sql_query(db);
					if (!sql_query.exec("create table if not exists Users( Name VARCHAR(10), UUID VARCHAR(56) PRIMARY KEY, Password VARCHAR(64));"))
					{
						//qDebug() << "Error: Fail to create table." << sql_query.lastError();
						db.close();
						inited_result = -3;
						return -3;
					}
				}

				//若config文件不存在
				{
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
		4.<-3>:插入数据出错
		5.<-4>:未成功创建用户组关联表
		6.<-5>:已注册
		*/
		if (inited == false)
		{
			return -2;
		}
		else
		{
			get_users();
			QString username = QString::fromLocal8Bit(Username.c_str());
			if (user_list.contains(username))
			{
				return -5;
			}
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
			QString cmd = "insert into Users values(?,?,?)";
			QString uuid = (QString)QCryptographicHash::hash(username.toLatin1(), QCryptographicHash::Sha3_224).toHex();
			sql_query.prepare(cmd);
			sql_query.addBindValue(username);
			sql_query.addBindValue(uuid);
			sql_query.addBindValue((QString)QCryptographicHash::hash(psw.toLatin1(), QCryptographicHash::Sha3_256).toHex());

			if (!sql_query.exec())
			{
				//qDebug() << sql_query.lastError() << endl;
				db.close();
				return -3;
			}
			if (!sql_query.exec("create table if not exists "+ uuid +"( GroupName VARCHAR PRIMARY KEY);"))
			{
				//qDebug() << "Error: Fail to create table." << sql_query.lastError();
				db.close();
				return -4;
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
		5.<-4>:未成功打开数据库搜索
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
		QString cmd = "select * from Users where Name='" + username + "' and UUID='" + (QString)QCryptographicHash::hash(username.toLatin1(), QCryptographicHash::Sha3_224).toHex() + "' and Password='" + (QString)QCryptographicHash::hash(psw.toLatin1(), QCryptographicHash::Sha3_256).toHex() + "'";
		sql_query.prepare(cmd);
		if (!sql_query.exec())
		{
			//qDebug() << "Error: Fail to select." << sql_query.lastError();
			db.close();
			return -4;
		}
		if (!sql_query.next())
		{
			db.close();
			return -3;
		}
		db.close();
		return 0;
	}
	//查看所有用户 *[返回QJsonObject user_list]
	QJsonObject get_users(void)
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
		QJsonObject psw_list;
		QSqlQuery query(db);
		query.exec("select * from Users");
		QSqlRecord rec = query.record();
		QString name;
		QString uuid;
		QString psw;
		while (query.next())
		{
			rec = query.record();
			uuid = query.value(rec.indexOf("UUID")).toString();
			name = query.value(rec.indexOf("Name")).toString();
			psw = query.value(rec.indexOf("Password")).toString();
			//qDebug() << "UserName:" << name;
			//qDebug() << "UUID:" << uuid;
			get.insert(name,uuid);
			psw_list.insert(uuid, psw);
		}
		db.close();
		user_list = get;
		user_psw = psw_list;
		return get;
	}
	//修改密码
	int chg_psw(std::string Username, std::string Psw="", std::string NewPsw = "")
	{
		/*
		[返回值说明]
		1.<0>:运行正常
		2.<-1>:未成功打开用户数据库
		3.<-2>:未初始化
		4.<-3>:未成功修改
		5.<-4>:不存在该用户
		6.<-5>:密码错误
		*/
		if (inited == false)
		{
			return -2;
		}
		//更新用户列表
		get_users();
		QString username = QString::fromLocal8Bit(Username.c_str());
		if (!user_list.contains(username))
		{
			return -4;
		}
		if (!user_psw.contains(user_list.value(username).toString()))
		{
			return -5;
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
		QString psw = QString::fromLocal8Bit(Psw.c_str());
		QString Npsw = QString::fromLocal8Bit(NewPsw.c_str());
		if (!query.exec("Update Users set Password='"+ (QString)QCryptographicHash::hash(Npsw.toLatin1(), QCryptographicHash::Sha3_256).toHex() +"' where Name='" + username + "' and UUID='" + (QString)QCryptographicHash::hash(username.toLatin1(), QCryptographicHash::Sha3_224).toHex() + "' and Password='" + (QString)QCryptographicHash::hash(psw.toLatin1(), QCryptographicHash::Sha3_256).toHex() + "'"))
		{
			return -3;
		}
		return 0;
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
		5.<-4>:不存在该用户
		6.<-5>:密码错误
		*/
		if (inited == false)
		{
			return -2;
		}
		//更新用户列表
		get_users();
		QString username = QString::fromLocal8Bit(Username.c_str());
		if (!user_list.contains(username))
		{
			return -4;
		}
		if (!user_psw.contains(user_list.value(username).toString()))
		{
			return -5;
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
		QString psw = QString::fromLocal8Bit(Psw.c_str());

		if (query.exec("delete from Users where Name='" + username + "' and UUID='" + user_list.value(username).toString() + "' and Password='" + (QString)QCryptographicHash::hash(psw.toLatin1(), QCryptographicHash::Sha3_256).toHex() + "'"))
		{
			get_usergroups(Username);
			int i = 0;
			int len = usergroup_list.length();
			QString groupname;
			for (i; i < len; ++i)
			{
				groupname = usergroup_list.value(QString::number(i)).toString();
				_remove_user(Username, groupname.toStdString());		
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
			db.open();
			if (!query.exec("drop table " + (QString)QCryptographicHash::hash(username.toLatin1(), QCryptographicHash::Sha3_224).toHex() +" ;"))
			{
				db.close();
				//未成功删除
				return -3;
			}
			
			get_users();
			return 0;
		}
		db.close();
		return -3;
	}
	//创建用户组
	int create_usergroup(std::string GroupName)
	{
		/*
		[返回值说明]
		1.<0>:运行正常
		2.<-1>:未成功打开数据库
		3.<-2>:未初始化
		4.<-3>:未成功创建用户组
		*/
		if (inited == false)
		{
			return -2;
		}
		//判断是否存在UserGroup连接
		if (QSqlDatabase::contains("UserGroup"))
		{
			db = QSqlDatabase::database("UserGroup");
		}
		else
		{
			db = QSqlDatabase::addDatabase("QSQLITE", "UserGroup");
			db.setDatabaseName(UserFilePath + "/UserGroup.lsf");
		}
		//打开数据库
		db.open();
		if (!db.isOpen())
		{
			//未打开用户数据库的情况
			return -1;
		}
		QString groupname = QString::fromLocal8Bit(GroupName.c_str());
		QSqlQuery sql_query(db);
		if (!sql_query.exec("create table if not exists "+groupname+"( Name VARCHAR(10), UUID VARCHAR(56) PRIMARY KEY);"))
		{
			//qDebug() << "Error: Fail to create table." << sql_query.lastError();
			db.close();
			return -3;
		}
		db.close();
		return 0;
	}
	//向用户组内添加用户
	int add_user(std::string Username, std::string GroupName)
	{
		/*
		[返回值说明]
		1.<0>:运行正常
		2.<-1>:未成功打开数据库
		3.<-2>:未初始化
		4.<-3>:不存在该用户
		5.<-4>:未成功添加用户
		6.<-5>:已注册 
		7.<-6>:未成功向用户组关联表添加组
		*/
		if (inited == false)
		{
			return -2;
		}
		//更新用户列表
		get_users();
		QString username = QString::fromLocal8Bit(Username.c_str());		
		if (!user_list.contains(username))
		{
			return -3;
		}
		QString groupname = QString::fromLocal8Bit(GroupName.c_str());
		//更新组内用户列表
		get_groupusers(GroupName);
		if (groupuser_list.contains(username))
		{
			return -5;
		}
		//判断是否存在UserGroup连接
		if (QSqlDatabase::contains("UserGroup"))
		{
			db = QSqlDatabase::database("UserGroup");
		}
		else
		{
			db = QSqlDatabase::addDatabase("QSQLITE", "UserGroup");
			db.setDatabaseName(UserFilePath + "/UserGroup.lsf");
		}
		//打开数据库
		db.open();
		if (!db.isOpen())
		{
			//未打开用户数据库的情况
			return -1;
		}
		QSqlQuery sql_query(db);
		if (!sql_query.exec("insert into " + groupname + " values('" + username + "','" + user_list.value(username).toString() + "')"))
		{
			//qDebug() << sql_query.lastError() << endl;
			db.close();
			return -4;
		}
		db.close();
		/************添加用户组记录************/
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
			//未打开用户数据库
			return -1;
		}
		QSqlQuery query(db);
		if (!query.exec("insert into "+ user_list.value(username).toString() + " values('" + groupname + "')"))
		{
			//qDebug() << query.lastError() << endl;
			db.close();
			return -6;
		}
		
		return 0;
	}
	//从组内删除用户
	int remove_user(std::string Username, std::string GroupName)
	{
		/*
		[返回值说明]
		1.<0>:运行正常
		2.<-1>:未成功打开数据库
		3.<-2>:未初始化
		4.<-3>:不存在该用户
		5.<-4>:未成功删除该用户
		6.<-5>:未成功删除记录
		*/
		if (inited == false)
		{
			return -2;
		}
		//更新用户列表
		get_groupusers(GroupName);
		QString groupname = QString::fromLocal8Bit(GroupName.c_str());
		QString username = QString::fromLocal8Bit(Username.c_str());
		if (groupuser_list.contains(username)==false)
		{
			return -3;
		}
		//判断是否存在UserGroup连接
		if (QSqlDatabase::contains("UserGroup"))
		{
			db = QSqlDatabase::database("UserGroup");
		}
		else
		{
			db = QSqlDatabase::addDatabase("QSQLITE", "UserGroup");
			db.setDatabaseName(UserFilePath + "/UserGroup.lsf");
		}
		//打开数据库
		db.open();
		if (!db.isOpen())
		{
			//未打开用户数据库的情况
			return -1;
		}
		QSqlQuery sql_query(db);
		if (!sql_query.exec("delete from "+groupname+" where Name='" + username + "' and UUID='" + groupuser_list.value(username).toString() + "'"))
		{
			//qDebug() << sql_query.lastError() << endl;
			db.close();
			return -4;
		}
		db.close();
		get_users();
		if (user_list.contains(username) == false)
		{
			return 0;
		}
		/************删除用户组记录************/
		{
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
				//未打开用户数据库
				return -1;
			}
			QSqlQuery query(db);
			if (!query.exec("delete from " + user_list.value(username).toString() + " where GroupName='" + groupname + "'"))
			{
				//qDebug() << query.lastError() << endl;
				db.close();
				return -5;
			}
		}
		return 0;
	}
	//查看组内用户 *[返回QJsonObject groupuser_list]
	QJsonObject get_groupusers(std::string GroupName)
	{
		QJsonObject get;
		if (inited == false)
		{
			return get;
		}
		//判断是否存在Users连接
		if (QSqlDatabase::contains("UserGroup"))
		{
			db = QSqlDatabase::database("UserGroup");
		}
		else
		{
			db = QSqlDatabase::addDatabase("QSQLITE", "UserGroup");
			db.setDatabaseName(UserFilePath + "/UserGroup.lsf");
		}
		//打开数据库
		db.open();
		if (!db.isOpen())
		{
			//未打开用户数据库的情况
			return get;
		}
		QJsonObject psw_list;
		QSqlQuery query(db);
		QString groupname = QString::fromLocal8Bit(GroupName.c_str());
		if (!query.exec("select * from " + groupname))
		{
			//qDebug() << query.lastError();
			return get;
		}
		
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
			get.insert(name, uuid);
		}
		db.close();
		groupuser_list = get;
		return get;
	}
	//查看用户关联的用户组 *[返回QJsonObject groupuser_list]
	QJsonObject get_usergroups(std::string Username)
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
		QString username = QString::fromLocal8Bit(Username.c_str());
		query.exec("select * from " + user_list.value(username).toString());
		QSqlRecord rec = query.record();
		QString name;
		QString uuid;
		int i = 0;
		while (query.next())
		{
			rec = query.record();
			uuid = query.value(rec.indexOf("GroupName")).toString();
			get.insert(QString::number(i), uuid);
			++i;
		}
		db.close();
		usergroup_list = get;
		return get;
	}
	//插入日志
	int write_log(std::string Info,std::string Utils="Utils" , std::string Type="INFO")
	{
		/*
		[返回值说明]
		1.<0>:运行正常
		2.<-1>:未成功打开日志文件
		3.<-2>:未初始化
		*/
		if (inited == false)
		{
			return -2;
		}

		if (!file_log.isOpen())
		{
			if (!file_log.open(QIODevice::Append))
			{
				return -1;
			}
		}
		QString info = QString::fromLocal8Bit(Info.c_str());
		QString utils = QString::fromLocal8Bit(Utils.c_str());
		QString type = QString::fromLocal8Bit(Type.c_str());
		QTextStream stream(&file_log);
		curDateTime = QDateTime::currentDateTime();
		stream << curDateTime.toString("yyyy-MM-dd hh:mm:ss") << " - " << utils << ": " << type << ":" << info << "\n";
		file_log.close();
		return 0;
	}

	/*析构函数*/
	~LinkerCore()
	{
		//备份用户数据
		if (File.exists(UserFilePath + "/Users.lsf.bak"))
		{
			Dir.remove(UserFilePath + "/Users.lsf.bak");
		}
		File.copy(UserFilePath + "/Users.lsf", UserFilePath + "/Users.lsf.bak");
		//备份用户组信息
		if (File.exists(UserFilePath + "/UserGroup.lsf.bak"))
		{
			Dir.remove(UserFilePath + "/UserGroup.lsf.bak");
		}
		File.copy(UserFilePath + "/UserGroup.lsf", UserFilePath + "/UserGroup.lsf.bak");
		if (file_log.isOpen())
		{
			file_log.close();
		}
	}
};