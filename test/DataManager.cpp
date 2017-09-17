#include "DataManager.h"
#include "../include/json/json.h"
#include "func.h"


CDataManager::CDataManager(string strHost, int port)
{
	m_bufferResultLen = 102400;
	m_bufferResultReceiveLen = 0;
	m_bufferResult = new char[m_bufferResultLen];

	core_init(strHost.c_str(),port);
	m_bThreadRuning = false;
}


CDataManager::~CDataManager(void)
{
	core_uninit();

	delete m_bufferResult;
	m_bufferResult = nullptr;
	m_bufferResultLen = 0;
}


void CDataManager::resetBuffer()
{
	m_bufferResultReceiveLen = m_bufferResultLen;
	m_bufferResult[0] = 0;
}

bool CDataManager::requestLogin(mgcHttpLogin &loginItem)
{
	while( thIsRunning() )
	{
		::Sleep(50);
	}

	m_bThreadRuning = true;
	m_enum_http_type = enum_http_login;

	m_dataloginItem = loginItem;

	//start thread 
	thStart();

	return true;
}

bool CDataManager::requestPhone()
{
	while( thIsRunning() )
	{
		::Sleep(50);
	}

	m_bThreadRuning = true;
	m_enum_http_type = enum_http_phone;

	//start thread 
	thStart();

	return true;
}

void CDataManager::processData()
{
	switch(m_enum_http_type)
	{
	case enum_http_login:
		{
			processData_login();
		}
		break;
	case enum_http_phone:
		{
			processData_phone();
		}
		break;
	default:
		break;
	}
	return;
}

void CDataManager::processData_login()
{
	//json parse
	string jsonData = m_bufferResult;

	Json::Reader reader;
	Json::Value json_object;
	
	if( !reader.parse(jsonData,json_object) )
		return;

	m_dataLoginResult.code = json_object["data"].asInt();
	m_dataLoginResult.msg = UTF8_To_string(json_object["returnMsg"].asString());
	m_dataLoginResult.token = json_object["token"].asString();

	int k = 0;
}

void CDataManager::processData_phone()
{
	//json parse
	string jsonData = m_bufferResult;

	Json::Reader reader;
	Json::Value json_object;
	
	if( !reader.parse(jsonData,json_object) )
		return;

	m_dataPhoneResult.code = json_object["data"].asInt();
	m_dataPhoneResult.msg = UTF8_To_string(json_object["returnMsg"].asString());
	m_dataPhoneResult.phone = json_object["phone"].asString();

	int k = 0;
}

void CDataManager::thread_main()
{
	resetBuffer();

	switch(m_enum_http_type)
	{
	case enum_http_login:
		{
		string strLoginUserName = m_dataloginItem.name;
		string strLoginPassword = m_dataloginItem.password;
		core_login(strLoginUserName.c_str(),strLoginPassword.c_str(),m_bufferResult,m_bufferResultReceiveLen);
		}
		break;
	case enum_http_phone:
		{
		core_get_service_phone_number(m_bufferResult,m_bufferResultReceiveLen);
		}
		break;
	default:
		break;
	}

	processData();
}