#include "stdafx.h"
#include "LogReader.h"
#include <fstream>
#include<iostream>

LogReader::LogReader(void)
{
	DWORD nCount = GetPrivateProfileString("DMS_CLIENT","LogFilePath","",logFileName,sizeof logFileName,"F:\\sky2000\\DMS\\common\\dms.ini");
	if (nCount==0)
	{
		printf("Failed to get log file name.\n");
		return;
	}
	nCount = GetPrivateProfileString("DMS_CLIENT","BackUpFileName","",backFileName,sizeof backFileName,"F:\\sky2000\\DMS\\common\\dms.ini");
	if (nCount==0)
	{
		printf("Failed to get back file name.\n");
		return;
	}
	nCount = GetPrivateProfileString("DMS_CLIENT","FailLoginsFileName","",failLoginsFileName,sizeof failLoginsFileName,"F:\\sky2000\\DMS\\common\\dms.ini");
	if (nCount==0)
	{
		printf("Failed to get fail log file name.\n");
		return;
	}
}


LogReader::~LogReader(void)
{
}

list<MatchedLogRec>& LogReader::readLogs()
{
	backUp();
	readFailedLogins();
	readBackUpFile();
	cout << "logins size=" << logins.size() << endl;
	cout << "logouts size=" << logouts.size() << endl;
	matchLogRec();
	cout << "logins size=" << logins.size() << endl;
	cout << "logouts size=" << logouts.size() << endl;
	cout << "matches size=" << readlog.size() << endl;
	saveFailLogins();
	rename(backFileName, logFileName);
	return readlog;
}
void LogReader::backUp()
{
	try{
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		char timeBuf[32];
		sprintf_s(timeBuf, "%04d年%02d月%02d日%02d时%02d分%02d秒", systime.wYear, systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond);
		strcat_s(backFileName, timeBuf);
		rename(logFileName,backFileName);
		
	}
	catch(...)
	{
		printf(_T("File %20s not found"), logFileName);
	}
}
void LogReader::readFailedLogins()
{
	ifstream ifs(failLoginsFileName);
	if (!ifs)
	{
		return;
	}
	LogRec log = { 0 };
	while (!ifs.eof())
	{
		ifs.read(reinterpret_cast<char*>(&log), sizeof log);
		logins.push_back(log);
		//std::cout << log.logname << ":"  <<"readFailedLogins"<< ":" << log.logtime << std::endl;
	}
}
void LogReader::readBackUpFile()
{
	FILE* pFile;
	fopen_s(&pFile, backFileName, "r");
	if (!pFile)
	{
		perror("failed to open backFile.");
		return;
	}
	size_t cycle_num;
	fseek(pFile, 0, SEEK_END);
	long flength = ftell(pFile);
	cycle_num = flength / 372;
	fseek(pFile, 0, SEEK_SET);
	for (size_t i = 0; i < 6; i++)
	{
		LogRec logrec = { 0 };
		fread(&logrec.logname, sizeof(char), 32, pFile);
		fseek(pFile, 36, SEEK_CUR);
		fread(&logrec.pid, sizeof(int), 1, pFile);
		//logrec.pid = htonl(logrec.pid);
		fread(&logrec.logtype, sizeof(short), 1, pFile);
		//logrec.logtype = htons(logrec.logtype);
		fseek(pFile, 6, SEEK_CUR);
		fread(&logrec.logtime, sizeof(int), 1, pFile);
		//logrec.logtime = htonl(logrec.logtime);
		fseek(pFile, 30, SEEK_CUR);
		fread(&logrec.logip, sizeof(char), 257, pFile);
		if (logrec.logname[0] != '.')
		{
			//std::cout << logrec.logname << ":" << cycle_num << ":" << flength << std::endl;
			if (logrec.logtype >= 2000)
			{
				logins.push_back(logrec);
			}
			else if (logrec.logtype < 2000)
			{
				logouts.push_back(logrec);
			}
		}
		fseek(pFile, 1, SEEK_CUR);
	}
	if (pFile)
	{
		fclose(pFile);
	}
	rename(backFileName, logFileName);
}
void LogReader::matchLogRec()
{
	for (LogRec & logout:logouts)
	{
		for (list<LogRec>::iterator i = logins.begin(); i != logins.end(); ++i)
		{
			if (strcmp(logout.logname, (*i).logname) == 0 && logout.pid == (*i).pid && !strcmp(logout.logip,(*i).logip))
			{
				MatchedLogRec log = { 0 };
				strcpy_s(log.logname, logout.logname);
				log.pid = logout.pid;
				log.logintime = logout.logtime;
				log.logouttime = (*i).logtime;
				log.duration = log.logouttime - log.logintime;
				strcpy_s(log.logip, logout.logip);
				logins.erase(i);
				readlog.push_back(log);
				break;
			}
		}
	}
	logouts.clear();
}
void LogReader::saveFailLogins()
{
	if (!logins.empty())
	{
		ofstream ofs;
		ofs.open(failLoginsFileName,ios::out|ios::binary|ios::trunc);
		if (!ofs)
		{
			perror("failed to open failLoginsFileName.");
			return;
		}
		for (LogRec & log : logins)
		{
			ofs.write(reinterpret_cast<char*>(&log), sizeof log);
			//std::cout << log.logname << ":" << "saveFailLogins" << ":" << log.logtime << std::endl;
		}
		logins.clear();
		ofs.close();
	}
}