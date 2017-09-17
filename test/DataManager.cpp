#include "DataManager.h"


CDataManager::CDataManager(string strHost, int port)
{
	m_bufferResultLen = 102400;
	m_bufferResult = new char[m_bufferResultLen];

	core_init("47.92.114.240",8080);

	m_bThreadRuning = false;
}


CDataManager::~CDataManager(void)
{
	core_uninit();

	delete m_bufferResult;
	m_bufferResult = nullptr;
	m_bufferResultLen = 0;
}


bool CDataManager::login(mgcHttpLogin &loginItem, mgcHttpLoginResult &result)
{
	while( thIsRunning() )
	{
		::Sleep(50);
	}
	//if( m_bThreadRuning )
	//	return false;

	m_bThreadRuning = true;
	m_enum_http_type = enum_http_login;

	/*
	string strLoginUserName = loginItem.name;
	string strLoginPassword = loginItem.password;
	//µÇÂ¼
	core_login(strLoginUserName.c_str(),strLoginPassword.c_str(),m_bufferResult,m_bufferResultLen);
	*/

	m_dataloginItem = loginItem;
	m_dataLoginResult = result;

	//start thread 
	thStart();

	/*
	while( thIsRunning() )
	{
		::Sleep(20);
	}

	//json parse
	string jsonData = m_bufferResult;

	m_bThreadRuning = false;
	*/
	return true;
}

void CDataManager::processData()
{
	switch(m_enum_http_type)
	{
	case enum_http_login:
		{
			//json parse
			string jsonData = m_bufferResult;
		}
		break;
	default:
		break;
	}
	return;
}

void CDataManager::thread_main()
{
	::Sleep(3000);
	switch(m_enum_http_type)
	{
	case enum_http_login:
		{
		string strLoginUserName = m_dataloginItem.name;
		string strLoginPassword = m_dataloginItem.password;
		core_login(strLoginUserName.c_str(),strLoginPassword.c_str(),m_bufferResult,m_bufferResultLen);
		}
		break;
	default:
		break;
	}

	processData();
}