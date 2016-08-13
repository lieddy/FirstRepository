#include "stdafx.h"
#include "LogReader.h"
#include <ctime>

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

list<MatchedLogRec> LogReader::readLogs()
{

	return logouts;
}
void LogReader::backUp()
{
	try{

		rename(logFileName,backFileName);
		
	}
	catch(CFileException* pEx)
	{
		TRACE(_T("File %20s not found, cause = %d\n"), logFileName, pEx->m_cause);
		pEx->Delete();
	}
}
void LogReader::readFailedLogins()
{
	CFile file;
	CFileException e;
	if (!file.Open(logFileName,CFile::modeRead,&e))
	{
		TCHAR ErrorMessage[1023];
		e.GetErrorMessage(&ErrorMessage,1023);
		TRACE("Fail to open log file: %1023s",ErrorMessage);
		return;
	}

}
void LogReader::readBackUpFile()
{

}
void LogReader::matchLogRec()
{

}
void LogReader::saveFailLogins()
{

}