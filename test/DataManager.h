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

	//��ά��Աlogin
	bool requestLogin(mgcHttpLogin &loginItem);
	bool getLoginData(mgcHttpLoginResult &result) {result = m_dataLoginResult;};

	//phone
	bool requestPhone();
	bool getPhoneData(mgcHttpPhoneResult &result) {result = m_dataPhoneResult;};

	//��ȡ�ͻ��˶�ά��
	bool requestQRCode();
	bool getQRCodeResult(mgcHttpQRCodeResult &result) {result = m_dataQRCodeResult;}

	//ȡ�����ļ� 
	bool requestDownFile(mgcHttpDownFile &downFile);
	bool getDownFileResult(mgcHttpDownFileResult &result) {result = m_dataDownFileResult;}

	//ȡ��Ϸ����
	bool requestAllGameFamily();
	bool getGameFamilyResult(mgcHttpGameFamilyResult &result) {result = m_dataGameFamilyResult;}

	//ȡ��Ϸ�б���Ϣ
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

	//��ǰ����
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

