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
* ��ʼ��/����ʼ��
*/
extern "C" LIBCORE_API bool core_init(const char *ip, int port);

extern "C" LIBCORE_API void core_uninit();


/**
* login
*/
extern "C" LIBCORE_API bool core_login(const char *user_name, const char* password, char * result, int& result_len);

/*
* ȡ�ͷ��绰
*/
extern "C" LIBCORE_API bool core_get_service_phone_number(char * result, int& result_len);

//ȡ�ͻ�����Ϣ
extern "C" LIBCORE_API bool core_get_client_device(char * client_id, char * result, int& result_len);

//ȡ��ά��
extern "C" LIBCORE_API bool core_get_client_qrcode(char * client_id, char * result, int& result_len, char *file_path);

//��ȡ��Ϸ�б�
//file_path���������png��·��������ɹ�����result,result_len����ֵ������.
//�����ص���json���ͣ��򲻻�����ָ���ļ�
//��У�����ɵ��ļ��Ƿ�Ϊ�Ϸ���png
extern "C" LIBCORE_API bool core_get_client_qrcode(char * client_id, char * result, int& result_len, char *file_path);


//��ȡ������Ϸ���
extern "C" LIBCORE_API bool core_get_game_type(int pageIndex, char * result, int& result_len);

//��ȡ������Ϸ�б�
extern "C" LIBCORE_API bool core_get_game_model(int pageIndex, char * result, int& result_len);