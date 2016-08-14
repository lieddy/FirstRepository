#pragma once
#include"../common/data.h"
#include<list>
class LogSender
{
public:
	LogSender(void);
	~LogSender(void);
	void SendMatches(std::list<MatchedLogRec>& matches);
	void ReadFailedSendFile(std::list<MatchedLogRec>& matches);
	void SaveFailedSendFile(std::list<MatchedLogRec>& matches);
	void InitNetWork();
private:
	char failSendFileName[50];
	unsigned short port;
	SOCKET sock;
	char serverIp[20];
};

