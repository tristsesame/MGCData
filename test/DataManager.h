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

	//运维人员login
	bool requestLogin(mgcHttpLogin &loginItem);
	bool getLoginData(mgcHttpLoginResult &result) {result = m_dataLoginResult;};

	//phone
	bool requestPhone();
	bool getPhoneData(mgcHttpPhoneResult &result) {result = m_dataPhoneResult;};

	//区取客户端二维码
	bool requestQRCode();
	bool getQRCodeResult(mgcHttpQRCodeResult &result) {result = m_dataQRCodeResult;}

	//取下载文件 
	bool requestDownFile(mgcHttpDownFile &downFile);
	bool getDownFileResult(mgcHttpDownFileResult &result) {result = m_dataDownFileResult;}

	//取游戏分类
	bool requestAllGameFamily();
	bool getGameFamilyResult(mgcHttpGameFamilyResult &result) {result = m_dataGameFamilyResult;}

	//取游戏列表信息
	bool requestAllGame();
	bool getGameResult(mgcHttpGameResult &result) {result = m_dataGameResult;}


	virtual void	thread_main();

private:
	bool requestGameFamily(mgcHttpGameFamily &item);
	bool requestGame(mgcHttpGame &item);

	void resetBuffer();

	void processData();
	void processData_login();
	void processData_phone();
	void processData_qrcode();
	void processData_down_file();
	void processData_game_family();
	void processData_game();

	char * m_bufferResult;
	int m_bufferResultReceiveLen;
	int m_bufferResultLen;

	bool m_bThreadRuning;

	bool m_bRequestReturn;

	//当前操作
	enum_http_request m_enum_http_type;

	//login
	mgcHttpLogin m_dataloginItem;
	mgcHttpLoginResult m_dataLoginResult;

	//phone
	mgcHttpPhoneResult m_dataPhoneResult;

	//QRCode 
	mgcHttpQRCode m_dataQRCode;
	mgcHttpQRCodeResult m_dataQRCodeResult;

	//down file
	mgcHttpDownFile m_dataDownFile;
	mgcHttpDownFileResult m_dataDownFileResult;

	//game family
	mgcHttpGameFamily m_dataGameFamily;
	mgcHttpGameFamilyResult m_dataGameFamilyResult;

	//game info list
	mgcHttpGame m_dataGame;
	mgcHttpGameResult m_dataGameResult;

};

