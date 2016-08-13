#pragma once
#include "data.h"
#include <list>
using namespace std;
class LogReader
{
public:
	LogReader(void);
	~LogReader(void);
	list<MatchedLogRec> readLogs();
protected:
	void backUp();
	void readFailedLogins();
	void readBackUpFile();
	void matchLogRec();
	void saveFailLogins();
private:
	char logFileName[50];
	char backFileName[50];
	char failLoginsFileName[50];
	list<LogRec> logins;
	list<MatchedLogRec> logouts;
};

