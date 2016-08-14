#pragma once
#include "../common/data.h"
#include <list>
using namespace std;
class LogReader
{
public:
	LogReader(void);
	~LogReader(void);
	list<MatchedLogRec>& readLogs();
private:
	void backUp();
	void readFailedLogins();
	void readBackUpFile();
	void matchLogRec();
	void saveFailLogins();
private:
	char logFileName[50];
	char backFileName[100];
	char failLoginsFileName[50];
	list<LogRec> logins;
	list<LogRec> logouts;
	list<MatchedLogRec> readlog;
};

