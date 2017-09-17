/*
*  2016.7.26
*  
*/

#pragma once

#include <utility> 
#include <vector>
#include <map>
#include <string>
#include <sstream>


using namespace std;

/*
所有的状态信息保存
*/

struct StatusInfo
{
	StatusInfo()
	{
		bInit = false;
	}

	//初始化是否成功
	bool bInit;
	std::string url;

};