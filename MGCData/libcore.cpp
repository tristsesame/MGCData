// libcore.cpp : 定义 DLL 应用程序的导出函数。
//

#include <time.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>

#include "libcore.h"
#include "core-info.h"
//#include <curl/curl.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>



#define WIN32_LEAN_AND_MEAN
#include <Dwmapi.h>

using namespace curlpp;

//保存数据
static StatusInfo status;
static char* dataBuffer = nullptr;
static size_t dataBufferSize = 100000;

//for write
static char* dataBufferPos = nullptr;
static size_t dataBufferReceiveSize = 0;

static 	string defaultHttpResultError = "{\"data\":-999,\"returnMsg\":\"服务器异常\"}";

//callback
void* Realloc(void* ptr, size_t size)
{
  if(ptr)
    return realloc(ptr, size);
  else
    return malloc(size);
};

void resetBuffer()
{
	dataBufferReceiveSize = 0;
	dataBufferPos = dataBuffer;
}

// Callback must be declared static, otherwise it won't link...
size_t WriteMemoryCallback(char* ptr, size_t size, size_t nmemb)
{
  // Calculate the real size of the incoming buffer
  size_t realsize = size * nmemb;
  
  // (Re)Allocate memory for the buffer
  if( dataBufferSize < dataBufferReceiveSize + realsize )
  {
		dataBufferSize = dataBufferReceiveSize + realsize + 1;
		dataBuffer = (char*) Realloc(dataBuffer, dataBufferSize);
		dataBufferPos = dataBuffer + dataBufferReceiveSize;
  }
  
  // Test if Buffer is initialized correctly & copy memory
  if (dataBuffer == NULL) {
    realsize = 0;
  }
  
  dataBufferPos = dataBuffer + dataBufferReceiveSize;
  memcpy(dataBufferPos, ptr, realsize);
  
  dataBufferReceiveSize += realsize;
  dataBuffer[dataBufferReceiveSize] = 0;
  cout << "dataBufferReceiveSize" << dataBufferReceiveSize << endl;
  // return the real size of the buffer...
  return realsize;
};


size_t FileCallback(FILE *f, char* ptr, size_t size, size_t nmemb)
{
	return fwrite(ptr, size, nmemb, f);
};


void print() 
{
	//std::cout << "Size: " << dataBufferReceiveSize << std::endl;
	//std::cout << "Content: " << std::endl << dataBuffer << std::endl;
}



bool is_init()
{
	return status.bInit;
}

bool http_request_get(const char *url,  char * result, int& result_len)
{
	resetBuffer();

	if ( result_len < 256 )
	{
		return false;
	}


	char * pBuffer = dataBuffer;
	pBuffer[0] = 0;

	try
	{
		curlpp::Cleanup cleaner;
		curlpp::Easy request;

		// Set the writer callback to enable cURL 
		// to write result in a memory area
		curlpp::types::WriteFunctionFunctor functor(WriteMemoryCallback);
		curlpp::options::WriteFunction *test = new curlpp::options::WriteFunction(functor);
		request.setOpt(test);

		// Setting the URL to retrive.
		request.setOpt(new curlpp::options::Url(url));
		request.setOpt(new curlpp::options::Verbose(true));
		request.setOpt(new curlpp::Options::Timeout(10));  //10秒超时
		request.setOpt(new curlpp::Options::ConnectTimeout(5));//连接超时，这个数值如果设置太短可能导致数据请求不到就断开了
		request.perform();

		//get the json string
		if( result_len < dataBufferReceiveSize + 1 )
		{
			return false;
		}
		else
		{
			memcpy(result,dataBuffer,dataBufferReceiveSize);
			result[dataBufferReceiveSize] = 0;
		}

		result_len = dataBufferReceiveSize;

		print();
	}
	catch ( curlpp::LogicError & e )
	{
		//std::cout << e.what() << std::endl;
	}
	catch ( curlpp::RuntimeError & e )
	{
		//std::cout << e.what() << std::endl;
	}

  return true;  
}



//dll export func
bool core_init( const char *ip, int port )
{
	char s[256];
	memset(s,0,256);
	sprintf(s, "http://%s:%d", ip,port);
	
	status.url = s;
	status.bInit = true;

	dataBuffer = (char*) malloc(dataBufferSize * sizeof(char));

	resetBuffer();
	return status.bInit;
}


void core_uninit()
{

}


bool core_login(const char *user_name, const char* password, char * result, int& result_len)
{
	char s[1024];
	memset(s,0,1024);
	sprintf(s,"%s/external/login?loginName=%s&loginPass=%s", status.url.c_str(),user_name,password);
	std::string strUrl = s;
	const char *url = strUrl.c_str();

	return http_request_get(url,result,result_len);
}

bool core_get_service_phone_number(char * result, int& result_len)
{
	char s[1024];
	memset(s,0,1024);
	sprintf(s,"%s/external/getServiceNum", status.url.c_str());
	std::string strUrl = s;
	const char *url = strUrl.c_str();

	return http_request_get(url,result,result_len);
}


bool core_get_client_device(char * client_id, char * result, int& result_len)
{
	char s[1024];
	memset(s,0,1024);
	sprintf(s,"%s/external/getClientDevice?clientId=%s", status.url.c_str(),client_id);
	std::string strUrl = s;
	const char *url = strUrl.c_str();

	return http_request_get(url,result,result_len);
}


bool core_get_client_qrcode(char * client_id, char * result, int& result_len, char *file_path)
{
	char s[1024];
	memset(s,0,1024);
	sprintf(s,"%s/external/getClientQRCode?clientId=%s", status.url.c_str(),client_id);
	std::string strUrl = s;
	const char *url = strUrl.c_str();

	if ( result_len < 256 )
	{
		return false;
	}

	char * pBuffer = dataBuffer;
	pBuffer[0] = 0;

	resetBuffer();
	try
	{
		curlpp::Cleanup cleaner;
		curlpp::Easy request;


		// Set the writer callback to enable cURL 
		// to write result in a memory area
		curlpp::types::WriteFunctionFunctor functor(WriteMemoryCallback);
		curlpp::options::WriteFunction *test = new curlpp::options::WriteFunction(functor);
		request.setOpt(test);

		// Setting the URL to retrive.
		request.setOpt(new curlpp::options::Url(url));
		request.setOpt(new curlpp::options::Verbose(true));
		request.setOpt(new curlpp::Options::Timeout(10));  //10秒超时
		request.setOpt(new curlpp::Options::ConnectTimeout(10));//连接超时，这个数值如果设置太短可能导致数据请求不到就断开了
		request.perform();

		//判断类型
		std::string contentType = curlpp::Infos::ContentType::get(request);

		//if( contentType == "contentType:image/png" )
		if( contentType.find("image/png") != std::string::npos )
		{
			FILE * file = fopen(file_path, "wb");
			if (!file)
			{
				std::cerr << "Error opening " << file_path << std::endl;
				return false;
			}
			fwrite(dataBuffer, dataBufferReceiveSize, 1, file);
			fclose(file);
		}

		//get the json string
		if( result_len < dataBufferReceiveSize + 1 )
		{
			return false;
		}
		else
		{
			if( contentType.find("json") != std::string::npos )
			{
				memcpy(result,dataBuffer,dataBufferReceiveSize);
				result[dataBufferReceiveSize] = 0;
				result_len = dataBufferReceiveSize;
			}
		}
		print();
	}
	catch ( curlpp::LogicError & e )
	{
		//std::cout << e.what() << std::endl;
	}
	catch ( curlpp::RuntimeError & e )
	{
		//std::cout << e.what() << std::endl;
	}
}

//获取游戏列表
bool core_get_game_type(int pageIndex, char * result, int& result_len)
{
	char s[1024];
	memset(s,0,1024);
	sprintf(s,"%s/external/findGameFamily?familyName=&familyNum=&pageNum=%d", status.url.c_str(),pageIndex);
	std::string strUrl = s;
	const char *url = strUrl.c_str();

	return http_request_get(url,result,result_len);
}

//获取游戏列表
bool core_get_game_model(int pageIndex, char * result, int& result_len)
{
	char s[1024];
	memset(s,0,1024);
	sprintf(s,"%s/external/findGameModel?gameName=&gameFamily=&operationMode=&pageNum=%d", status.url.c_str(),pageIndex);
	std::string strUrl = s;
	const char *url = strUrl.c_str();

	return http_request_get(url,result,result_len);
}