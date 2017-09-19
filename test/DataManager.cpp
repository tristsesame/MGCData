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

bool CDataManager::requestQRCode()
{
	while( thIsRunning() )
	{
		::Sleep(50);
	}

	m_bThreadRuning = true;
	m_enum_http_type = enum_http_qrcode;

	//start thread 
	thStart();

	return true;
}

bool CDataManager::requestDownFile(mgcHttpDownFile &downFile)
{
	while( thIsRunning() )
	{
		::Sleep(50);
	}

	m_bThreadRuning = true;
	m_enum_http_type = enum_http_down_file;

	//start thread 
	thStart();

	return true;
}

bool CDataManager::requestAllGameFamily()
{
	mgcHttpGameFamily item;
	item.pageNum = 1;
	requestGameFamily(item);

	return true;
}

bool CDataManager::requestGameFamily(mgcHttpGameFamily &item)
{
	while( thIsRunning() )
	{
		::Sleep(50);
	}

	m_bThreadRuning = true;
	m_enum_http_type = enum_http_game_family;

	m_dataGameFamily = item;

	//start thread 
	thStart();

	return true;
}

bool CDataManager::requestAllGame()
{
	mgcHttpGame item;
	item.pageNum = 1;
	requestGame(item);

	return true;
}

bool CDataManager::requestGame(mgcHttpGame &item)
{
	while( thIsRunning() )
	{
		::Sleep(50);
	}

	m_bThreadRuning = true;
	m_enum_http_type = enum_http_game;

	m_dataGame = item;

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
	case enum_http_qrcode:
		{
			processData_qrcode();
		}
		break;
	case enum_http_down_file:
		{
			processData_down_file();
		}
		break;
	case enum_http_game_family:
		{
			processData_game_family();
		}
		break;
	case enum_http_game:
		{
			processData_game();
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
	m_dataLoginResult.operationId = json_object["operationId"].asString();
	m_dataLoginResult.operationName = json_object["operationName"].asString();
	m_dataLoginResult.operationCode = json_object["operationCode"].asString();

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
}

void CDataManager::processData_qrcode()
{
	if (m_bufferResultReceiveLen == 0)
	{
		//认为成功
		m_dataPhoneResult.code = 1;
		return;
	}

	//json parse
	string jsonData = m_bufferResult;

	Json::Reader reader;
	Json::Value json_object;
	
	if( !reader.parse(jsonData,json_object) )
		return;

	m_dataPhoneResult.code = json_object["data"].asInt();
	m_dataPhoneResult.msg = UTF8_To_string(json_object["returnMsg"].asString());
}

void CDataManager::processData_down_file()
{
	if (m_bRequestReturn)
	{
		//认为成功
		m_dataPhoneResult.code = 1;
	}
	else
	{
		//失败
		m_dataPhoneResult.code = 0;
	}
}

void CDataManager::processData_game_family()
{
	//json parse
	string jsonData = m_bufferResult;

	Json::Reader reader;
	Json::Value json_object;
	
	if( !reader.parse(jsonData,json_object) )
		return;

	m_dataGameFamilyResult.code = json_object["data"].asInt();
	m_dataGameFamilyResult.msg = UTF8_To_string(json_object["returnMsg"].asString());
	m_dataGameFamilyResult.pageSize = json_object["pageSize"].asInt();
	m_dataGameFamilyResult.totalResult = json_object["totalResult"].asInt();

	//json array
	Json::Value items = json_object["dataList"];
	int items_size = items.size();

	for(size_t i = 0; i < items_size; i++)
	{
		string strFamilyId = UTF8_To_string(items[i]["familyId"].asString());
		if( m_dataGameFamilyResult.mapFamilyId.find(strFamilyId) != m_dataGameFamilyResult.mapFamilyId.end() )
		{
			continue;
		}

		//add
		m_dataGameFamilyResult.mapFamilyId[strFamilyId] = true;

		mgcGameFamilyInfo info;
		info.familyId = strFamilyId;
		info.familyNum = UTF8_To_string(items[i]["familyNum"].asString());
		info.familyName = UTF8_To_string(items[i]["familyName"].asString());
		info.familyIcon = UTF8_To_string(items[i]["familyIcon"].asString());



		m_dataGameFamilyResult.vecFamilyInfo.push_back(info);
	}


	//判断是否已经取完
	if( m_dataGameFamily.pageNum < m_dataGameFamilyResult.pageSize )
	{
		mgcHttpGameFamily item;
		item.pageNum++;
		requestGameFamily(item);
	}
}

void CDataManager::processData_game()
{
	//json parse
	string jsonData = m_bufferResult;

	Json::Reader reader;
	Json::Value json_object;
	
	if( !reader.parse(jsonData,json_object) )
		return;

	m_dataGameResult.code = json_object["data"].asInt();
	m_dataGameResult.msg = UTF8_To_string(json_object["returnMsg"].asString());
	m_dataGameResult.pageSize = json_object["pageSize"].asInt();
	m_dataGameResult.totalResult = json_object["totalResult"].asInt();

	//json array
	Json::Value items = json_object["dataList"];
	int items_size = items.size();

	for(size_t i = 0; i < items_size; i++)
	{
		string strGameId = UTF8_To_string(items[i]["gameId"].asString());
		if( m_dataGameResult.mapGameId.find(strGameId) != m_dataGameResult.mapGameId.end() )
		{
			continue;
		}

		//add
		m_dataGameResult.mapGameId[strGameId] = true;

		mgcGameInfo info;

			struct mgcGameInfo
	{
		string gameId;
		string gameName;
		string gamePath;
		string gameFamily;
		string operationMode;
		string gameCover;
		string gameVideo;
	};

		info.gameId = strGameId;
		info.gameName = UTF8_To_string(items[i]["gameName"].asString());
		info.gamePath = UTF8_To_string(items[i]["gamePath"].asString());
		info.gameFamily = UTF8_To_string(items[i]["gameFamily"].asString());
		info.operationMode = UTF8_To_string(items[i]["operationMode"].asString());
		info.gameCover = UTF8_To_string(items[i]["gameCover"].asString());
		info.gameVideo = UTF8_To_string(items[i]["gameVideo"].asString());

		m_dataGameResult.vecGameInfo.push_back(info);
	}


	//判断是否已经取完
	if( m_dataGame.pageNum < m_dataGameResult.pageSize )
	{
		mgcHttpGame item;
		item.pageNum++;
		requestGame(item);
	}
}

void CDataManager::thread_main()
{
	resetBuffer();

	bool bRtn;
	switch(m_enum_http_type)
	{
	case enum_http_login:
		{
		string strLoginUserName = m_dataloginItem.name;
		string strLoginPassword = m_dataloginItem.password;
		bRtn = core_login(strLoginUserName.c_str(),strLoginPassword.c_str(),m_bufferResult,m_bufferResultReceiveLen);
		}
		break;
	case enum_http_phone:
		{
		bRtn = core_get_service_phone_number(m_bufferResult,m_bufferResultReceiveLen);
		}
		break;
	case enum_http_qrcode:
		{
			bRtn = core_get_client_qrcode(m_dataQRCode.clientId.c_str(),m_bufferResult,\
				m_bufferResultReceiveLen,m_dataQRCode.filePath.c_str());
		}
		break;
	case enum_http_down_file:
		{
			bRtn = core_down_file(m_dataDownFile.fileUrl.c_str(), m_dataDownFile.filePath.c_str());
		}
		break;
	case enum_http_game_family:
		{
			//除pgaeNum外，其它参数暂时为空，表示取所有
			bRtn = core_get_game_type(m_dataGameFamily.pageNum,m_bufferResult,m_bufferResultReceiveLen);
		}
		break;
	case enum_http_game:
		{
			//除pgaeNum外，其它参数暂时为空，表示取所有
			bRtn = core_get_game_model(m_dataGame.pageNum,m_bufferResult,m_bufferResultReceiveLen);
		}
		break;
	default:
		break;
	}

	m_bRequestReturn = bRtn;
	processData();
}