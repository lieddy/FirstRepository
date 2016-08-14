// client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "LogReader.h"
#include"LogSender.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//WritePrivateProfileString("DMS_CLIENT","LogFilePath","F:\\sky2000\\DMS\\common\\wtmpx","F:\\sky2000\\DMS\\common\\dms.ini");
// 	WritePrivateProfileString("DMS_CLIENT","BackUpFileName","F:\\sky2000\\DMS\\common\\backup","F:\\sky2000\\DMS\\common\\dms.ini");
// 	WritePrivateProfileString("DMS_CLIENT","FailLoginsFileName","F:\\sky2000\\DMS\\common\\failLogin","F:\\sky2000\\DMS\\common\\dms.ini");
	LogReader reader;
	LogSender sender;
	list<MatchedLogRec> list = reader.readLogs();
	sender.SendMatches(list);
	return 0;
}

