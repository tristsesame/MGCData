
#include <string>
#include <iostream>
#include <fstream>
#include "libcore.h"
#include "DataManager.h"

using namespace std;

int main()
{
	CDataManager data("47.92.114.240",8080);
	mgcHttpLogin loginItem;
	mgcHttpLoginResult loginResult;

	loginItem.name = "admin";
	loginItem.password = "super";

	data.requestLogin(loginItem);

	data.requestPhone();
	
	while( true )
	{
		cout << "I'm tired, sleep for a while ... " << endl;
		Sleep(200);
	}

	return 1;

	//this is the break line
	//-------------------------------------------------------------------------------

	// dll test
	char result[102400];
	int len = 1024;


	//test 

	core_init("47.92.114.240",8080);

	//登录
	core_login("admin","super",result,len);
	std::cout << "login result len:" << len << endl;
	std::cout << "login result:" << result << endl;

	//取客服电话
	len = 1024;
	core_get_service_phone_number(result,len);
	std::cout << "get service phone result len:" << len << endl;
	std::cout << "get service phone result:" << result << endl;

	//获取客户端信息
	len = 1024;
	core_get_client_device("ddsfdsfdsf",result,len);
	std::cout << "result: get client device result len:" << len << endl;
	std::cout << "result: get client device:" << result << endl;

	//取得二维码
	len = 1024;
	core_get_client_qrcode("ddsfdsfdsf",result,len,"C:\\Users\\sesame\\Pictures\\testCode.png");
	std::cout << "result: get client qr code len:" << len << endl;
	std::cout << "result: get client qr code:" << result << endl;

	//取得游戏类别列表
	len = 1024;
	core_get_game_type(1,result,len);
	std::cout << "result: get game_type len:" << len << endl;
	std::cout << "result: get game_type:" << result << endl;

	//取得游戏类别列表
	len = 1024;
	core_get_game_model(1,result,len);
	std::cout << "result: get game_model len:" << len << endl;
	std::cout << "result: get game_model:" << result << endl;


	return 0;
}