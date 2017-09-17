#pragma once

#include <string>

using namespace std;
namespace MGCDataDefine
{
	enum enum_http_request
	{
		enum_http_login = 0,
		enum_http_phone_number,
	};

	//base
	struct mgcRequestResult
	{
		mgcRequestResult()
		{
			code = -1; //data exception
			msg = "";  //MultiByte
		}
		int code;
		string msg;
	};

	//login item
	struct mgcHttpLogin
	{
		string name;
		string password;
	};

	struct mgcHttpLoginResult:mgcRequestResult
	{
		mgcHttpLoginResult()
		{
			token = "";
		}
		string token;
	};

	//other item

}