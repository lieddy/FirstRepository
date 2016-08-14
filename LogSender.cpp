#include<Windows.h>
#include "LogSender.h"
//#include<WinSock2.h>
#include<fstream>
#include "exception.h"
#pragma comment(lib,"Ws2_32.lib")
LogSender::LogSender(void)
{
	::GetPrivateProfileString("DMS_CLIENT", "FailSendFileName", "", failSendFileName, sizeof failSendFileName, "F:\\sky2000\\DMS\\common\\dms.ini");
	if (strlen(failSendFileName)==0)
	{
		printf("fail to get failSendFileName.");
	}
}


LogSender::~LogSender(void)
{
	WSACleanup();
}
void LogSender::SendMatches(std::list<MatchedLogRec>& matches) throw(DmsSendDataException,DmsInitNetWorkException)
{
	try
	{
		InitNetWork();
	}
	catch (DmsInitNetWorkException& e)
	{
		printf(e.what());
		throw;
	}
	ReadFailedSendFile(matches);
	while (matches.size() > 0)
	{
		int sfd = send(sock, reinterpret_cast<char*>(&(*matches.begin())), sizeof(MatchedLogRec), MSG_DONTROUTE);
		if (sfd > 0)
		{
			matches.erase(matches.begin());
		}
		else
		{
			throw(DmsSendDataException("send data failed!"));
		}
	}
	SaveFailedSendFile(matches);
}
void LogSender::ReadFailedSendFile(std::list<MatchedLogRec>& matches)
{
	std::ifstream ifs(failSendFileName);
	if (!ifs)
	{
		printf("fail to open failSendFileName in ReadFailedSendFile.\n");
		return;
	}
	MatchedLogRec matchRec;
	while (!ifs.eof())
	{
		ifs.read(reinterpret_cast<char*>(&matchRec), sizeof matchRec);
		matches.push_back(matchRec);
	}
	ifs.close();
}
void LogSender::SaveFailedSendFile(std::list<MatchedLogRec>& matches)
{
	std::ofstream ofs(failSendFileName, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!ofs)
	{
		printf("fail to open failSendFile.\n");
		return;
	}
	for (MatchedLogRec& log : matches)
	{
		ofs.write(reinterpret_cast<char*>(&log), sizeof log);
	}
	matches.clear();
	ofs.close();
}
void LogSender::InitNetWork()throw(DmsInitNetWorkException)
{
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("failt to call WSAStartup.");
		throw;
	}
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		printf("failed to create socket.\n");
		WSACleanup();
		throw;
	}
	port = htons(8000);
	strcpy_s(serverIp, "127.0.0.1");
	SOCKADDR_IN svrAddr;
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = port;
	svrAddr.sin_addr.S_un.S_addr = inet_addr(serverIp);
	int cfd=connect(sock, (sockaddr*)&svrAddr, sizeof(svrAddr));
	if (cfd==SOCKET_ERROR)
	{
		throw;
	}
}