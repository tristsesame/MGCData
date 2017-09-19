/*
2017/09/16
*/

#pragma once

#ifdef LIBCORE_EXPORTS
#define LIBCORE_API __declspec(dllexport)
#else
#define LIBCORE_API __declspec(dllimport)
#endif

/**
* init & uninit
* 初始化/反初始化
*/
extern "C" LIBCORE_API bool core_init(const char *ip, int port);

extern "C" LIBCORE_API void core_uninit();


/**
* login
*/
extern "C" LIBCORE_API bool core_login(const char *user_name, const char* password, char * result, int& result_len);

/*
* 取客服电话
*/
extern "C" LIBCORE_API bool core_get_service_phone_number(char * result, int& result_len);

//取客户端信息
extern "C" LIBCORE_API bool core_get_client_device(const char * client_id, char * result, int& result_len);

//获取游戏列表
//file_path传欲保存的png的路径，如果成功，则result,result_len二个值不处理.
//若返回的是json类型，则不会生成指定文件
//需校验生成的文件是否为合法的png
extern "C" LIBCORE_API bool core_get_client_qrcode(const char * client_id, char * result, int& result_len, const char *file_path);


//获取所有游戏类别
extern "C" LIBCORE_API bool core_get_game_type(int pageIndex, char * result, int& result_len);

//获取所有游戏列表
extern "C" LIBCORE_API bool core_get_game_model(int pageIndex, char * result, int& result_len);

//客户端信息上报
extern "C" LIBCORE_API bool core_save_client_device(const char * client_id, const char * mac, char * result, int& result_len);

//下载文件 
extern "C" LIBCORE_API bool core_down_file(const char* file_url,const char * file_path);