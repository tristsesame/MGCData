#pragma once

#include <Windows.h>
#include <string>
#include "threadwrap.h"
#include "Define.h"
#include "libcore.h"

using namespace std;
using namespace MGCDataDefine;

class CDataManager :
	public xdmp::ThreadWrap
{
public:
	CDataManager(string strHost, int port);
	~CDataManager(void);

	bool login(mgcHttpLogin &loginItem, mgcHttpLoginResult &result);
	virtual void	thread_main();

private:
	void processData();

	char * m_bufferResult;
	int m_bufferResultLen;

	bool m_bThreadRuning;

	//��ǰ����
	enum_http_request m_enum_http_type;

	//login
	mgcHttpLogin m_dataloginItem;
	mgcHttpLoginResult m_dataLoginResult;

};

