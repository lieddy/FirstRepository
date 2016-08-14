#pragma once
#include<string>
using namespace std;

class DmsException
{
public:
	DmsException(string msg="Dms exception") :m_msg(msg) {}
	virtual const char* what()
	{
		return m_msg.c_str();
	}
private:
	string m_msg;
};
class DmsSendException:public DmsException
{
public:
	DmsSendException(string msg);
	~DmsSendException();

};

DmsSendException::DmsSendException(string msg="Dms Send Exception"):DmsException(msg)
{
}

DmsSendException::~DmsSendException()
{
}
class DmsSendDataException:public DmsSendException
{
public:
	DmsSendDataException(string msg);
	~DmsSendDataException();
};

DmsSendDataException::DmsSendDataException(string msg="Dms Send Data Exception"):DmsSendException(msg)
{
}

DmsSendDataException::~DmsSendDataException()
{
}
class DmsInitNetWorkException:public DmsSendException
{
public:
	DmsInitNetWorkException(string msg);
	~DmsInitNetWorkException();
};

DmsInitNetWorkException::DmsInitNetWorkException(string msg="Dms Init Network Exception."):DmsSendException(msg)
{
}

DmsInitNetWorkException::~DmsInitNetWorkException()
{
}