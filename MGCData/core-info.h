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
���е�״̬��Ϣ����
*/

struct StatusInfo
{
	StatusInfo()
	{
		bInit = false;
	}

	//��ʼ���Ƿ�ɹ�
	bool bInit;
	std::string url;

};